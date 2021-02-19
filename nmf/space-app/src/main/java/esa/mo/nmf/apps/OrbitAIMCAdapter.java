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
import esa.mo.nmf.MonitorAndControlNMFAdapter;
import esa.mo.nmf.annotations.Parameter;
import esa.mo.nmf.commonmoadapter.SimpleDataReceivedListener;
import esa.mo.nmf.spacemoadapter.SpaceMOApdapterImpl;

/**
 * Monitoring and control interface of the OrbitAI space application. Handles parameters and
 * actions.
 */
public class OrbitAIMCAdapter extends MonitorAndControlNMFAdapter {

  private static final Logger LOGGER = Logger.getLogger(OrbitAIMCAdapter.class.getName());

  /**
   * Relative path to the file containing the list of OBSW parameters we want to subscribe to from
   * supervisor.
   */
  private static final String PARAM_SUBS_FILE_PATH = "subscriptions.txt";

  // ----------------------------------- Parameters -----------------------------------------------

  private static final float PARAMS_DEFAULT_VALUE = 42;

  /**
   * The listener for parameters values coming from supervisor.
   */
  private SimpleDataReceivedListener parameterListener;

  // -------- RE-EXPOSING some parameters consumed from supervisor --------

  // CADC

  // CADC PHOTODIOES parameters

  @Parameter(description = "I_PD3_THETA fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0888 = PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD6_THETA fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0894 = PARAMS_DEFAULT_VALUE;

  // CADC QUATERNION parameters

  @Parameter(description = "O_Q_FB_FI_EST_0 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC1002 = PARAMS_DEFAULT_VALUE;

  @Parameter(description = "O_Q_FB_FI_EST_1 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC1003 = PARAMS_DEFAULT_VALUE;

  @Parameter(description = "O_Q_FB_FI_EST_2 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC1004 = PARAMS_DEFAULT_VALUE;

  @Parameter(description = "O_Q_FB_FI_EST_3 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC1005 = PARAMS_DEFAULT_VALUE;


  /**
   * Queries OBSW parameters values from the supervisor and set our internal parameters with those
   * values.
   */
  public void toggleSupervisorParametersSubscription(SpaceMOApdapterImpl supervisorSMA,
      boolean subscribe) {
    supervisorSMA.toggleParametersGeneration(getParametersToSubscribeTo(), subscribe);

    if (subscribe) {
      // Only if it's the first time we try to subscribe, create the listener and add it
      if (this.parameterListener == null) {
        this.parameterListener = new SimpleDataReceivedListener() {
          @Override
          public void onDataReceived(String parameterName, Serializable data) {
            if (data == null) {
              LOGGER.log(Level.WARNING,
                  String.format("Received null value for parameter %s", parameterName));
              return;
            }

            String dataS = data.toString();
            LOGGER.log(Level.INFO, String.format(
                "Received value %s from supervisor for parameter %s", dataS, parameterName));

            try {
              Field internalParameter =
                  OrbitAIMCAdapter.this.getClass().getDeclaredField(parameterName);
              internalParameter.setAccessible(true);
              internalParameter.set(OrbitAIMCAdapter.this, Float.parseFloat(dataS));
              internalParameter.setAccessible(false);

            } catch (NoSuchFieldException e) { // received parameter that we don't need
              LOGGER.log(Level.WARNING,
                  String.format("Value %s received from supervisor for parameter %s was not used",
                      dataS, parameterName));
            } catch (SecurityException | IllegalArgumentException | IllegalAccessException e1) {
              LOGGER.log(Level.SEVERE,
                  "Error setting internal parameter from supervisor acquired value", e1);
            }
          }
        };

        supervisorSMA.addDataReceivedListener(this.parameterListener);
      }
    }
  }

  /**
   * Parses the parameter subscriptions file and returns the list of OBSW parameter we want to
   * subscribe to from supervisor.
   *
   * @return The list of parameter identifiers.
   */
  private List<String> getParametersToSubscribeTo() {
    List<String> paramNames = new ArrayList<String>();
    File file = new File(PARAM_SUBS_FILE_PATH);

    if (file.exists()) {
      try {
        // expect one line with parameters names
        String content = new String(Files.readAllBytes(file.toPath()));
        content = content.replace("\n", "");

        for (String paramName : content.split(",")) {
          paramNames.add(paramName);
          LOGGER.log(Level.INFO,
              String.format("Will subscribe to parameter %s from supervisor", paramName));
        }
      } catch (IOException e) {
        LOGGER.log(Level.SEVERE, "Error while loading parameters to subscribe to", e);
      }
    } else {
      LOGGER.log(Level.SEVERE,
          String.format("Trying to load file %s that doesn't exist", file.getPath()));
    }

    return paramNames;
  }
}
