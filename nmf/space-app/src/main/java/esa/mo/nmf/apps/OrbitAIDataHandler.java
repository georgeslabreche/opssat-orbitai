// (C) 2021 European Space Agency
// European Space Operations Centre
// Darmstadt, Germany

package esa.mo.nmf.apps;

import java.io.File;
import java.io.IOException;
import java.io.Serializable;
import java.lang.reflect.Field;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.ccsds.moims.mo.mal.structures.UInteger;
import esa.mo.nmf.NMFException;
import esa.mo.nmf.commonmoadapter.SimpleDataReceivedListener;

/**
 * Handles tasks related to data: fetch data from supervisor, save the data, prepare them for the
 * training.
 *
 * @author Tanguy Soto
 */
public class OrbitAIDataHandler {

  private static final Logger LOGGER = Logger.getLogger(OrbitAITrainingHandler.class.getName());

  /**
   * Relative path to the file containing the list of OBSW parameters we want to subscribe to from
   * supervisor.
   */
  private static final String PARAM_SUBS_FILE_PATH = "subscriptions.txt";

  /**
   * M&C interface of the application.
   */
  private final OrbitAIMCAdapter adapter;

  /**
   * The listener for parameters values coming from supervisor.
   */
  private SimpleDataReceivedListener parameterListener;


  public OrbitAIDataHandler(OrbitAIMCAdapter adapter) {
    this.adapter = adapter;
  }

  /**
   * Queries OBSW parameters values from the supervisor and set our internal parameters with those
   * values.
   * 
   * @param subscribe True if we want supervisor to push new parameters data, false
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public UInteger toggleSupervisorParametersSubscription(boolean subscribe) {
    List<String> parametersNames = getParametersToSubscribeTo();
    if (parametersNames == null) {
      return new UInteger(1);
    }
    if (parametersNames.size() <= 0) {
      return new UInteger(2);
    }

    // Always toggle the parameters generation in supervisor
    try {
      adapter.getSupervisorSMA().toggleParametersGeneration(parametersNames, subscribe);
    } catch (NMFException e0) {
      LOGGER.log(Level.SEVERE, "Error toggling supervisor parameters generation", e0);
      return new UInteger(3);
    }

    // Only if first time, create and register the parameters listener
    if (subscribe && this.parameterListener == null) {
      this.parameterListener = new SimpleDataReceivedListener() {
        @Override
        public void onDataReceived(String parameterName, Serializable data) {
          if (data == null) {
            LOGGER.log(Level.WARNING,
                String.format("Received null value for parameter %s", parameterName));
            return;
          }

          String dataS = data.toString();
          LOGGER.log(Level.INFO, String.format("Received value %s from supervisor for parameter %s",
              dataS, parameterName));

          try {
            // update internal parameter with the value received
            Field internalParameter = adapter.getClass().getDeclaredField(parameterName);
            internalParameter.setAccessible(true);
            internalParameter.set(adapter, Float.parseFloat(dataS));
            internalParameter.setAccessible(false);

          } catch (NoSuchFieldException e1) {
            // received parameter value that we don't need
            LOGGER.log(Level.WARNING,
                String.format("Value %s received from supervisor for parameter %s was not used",
                    dataS, parameterName));
          } catch (SecurityException | IllegalArgumentException | IllegalAccessException e2) {
            LOGGER.log(Level.SEVERE,
                "Error setting internal parameter from supervisor acquired value", e2);
          }
        }
      };

      adapter.getSupervisorSMA().addDataReceivedListener(this.parameterListener);
    }

    return null;
  }
  
  /**
   * 
   * TODO getTrainingDataSet
   *
   */
  public void getTrainingDataSet() {
    
  }

  /**
   * Parses the parameter subscriptions file and returns the list of OBSW parameter we want to
   * subscribe to from supervisor.
   *
   * @return The list of parameter identifiers or null if an IOException occurred.
   */
  private List<String> getParametersToSubscribeTo() {
    File file = new File(PARAM_SUBS_FILE_PATH);
    String content = "";

    try {
      // expect one line with parameters names
      content = new String(Files.readAllBytes(file.toPath()));
    } catch (IOException e) {
      LOGGER.log(Level.SEVERE, "Error while loading parameters to subscribe to", e);
      return null;
    }

    // parse the line
    List<String> paramNames = new ArrayList<String>();
    content = content.replace("\n", "");
    for (String paramName : content.split(",")) {
      paramNames.add(paramName);
    }

    if (paramNames.size() <= 0) {
      LOGGER.log(Level.WARNING,
          String.format("Parameters names list read from %s is empty", file.toPath()));
    }
    return paramNames;
  }
}
