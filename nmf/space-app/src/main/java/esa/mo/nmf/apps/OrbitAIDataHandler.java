// (C) 2021 European Space Agency
// European Space Operations Centre
// Darmstadt, Germany

package esa.mo.nmf.apps;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.Serializable;
import java.nio.file.Paths;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.locks.ReentrantLock;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.ccsds.moims.mo.mal.structures.UInteger;
import esa.mo.nmf.NMFException;
import esa.mo.nmf.commonmoadapter.SimpleDataReceivedListener;

/**
 * Handles tasks related to data: fetch data from supervisor, save the data.
 *
 * @author Tanguy Soto
 */
public class OrbitAIDataHandler {

  private static final Logger LOGGER = Logger.getLogger(OrbitAIDataHandler.class.getName());

  /**
   * Relative path to the directory containing our data inside the toGround/ folder.
   */
  private static final String DATA_DIR = "data";

  /**
   * Time stamps format for data logs.
   */
  private static final SimpleDateFormat timestampFormat =
      new SimpleDateFormat("yyyy-MM-dd_HH-mm-ss");

  /**
   * Photodiode elevation threshold for the HD Camera: FOV 18.63 deg (in lens specs) and 21 deg (in
   * ICD). Elevation threshold is 90 deg - (FOV + margin) = 60 deg (1.0472 rad).
   */
  private static final float PD6_ELEVATION_THRESHOLD_HD_CAM = 1.0472f;

  /**
   * M&C interface of the application.
   */
  private final OrbitAIMCAdapter adapter;

  /**
   * Parameters default value before first acquisition.
   */
  private static final String PARAMS_DEFAULT_VALUE = "x";

  /**
   * Lock for accessing our internal parameters
   */
  private final ReentrantLock parametersLock = new ReentrantLock();

  /**
   * Latest parameters values acquired from supervisor.
   */
  private final HashMap<String, String> parameters;

  /**
   * The listener for parameters values coming from supervisor.
   */
  private SimpleDataReceivedListener parameterListener;

  /**
   * File stream used to log our acquired data.
   */
  private FileOutputStream trainingDataOutputStream;

  /**
   * Storing supervisor parameters names for convenience.
   */
  public static final String PD1 = "CADC0884";
  public static final String PD1_FLAG = "CADC0885";
  public static final String PD2 = "CADC0886";
  public static final String PD2_FLAG = "CADC0887";
  public static final String PD3 = "CADC0888";
  public static final String PD3_FLAG = "CADC0889";
  public static final String PD4_FLAG = "CADC0891";
  public static final String PD4 = "CADC0890";
  public static final String PD5 = "CADC0892";
  public static final String PD5_FLAG = "CADC0893";
  public static final String PD6 = "CADC0894";
  public static final String PD6_FLAG = "CADC0895";

  private static List<String> parametersNames;


  public OrbitAIDataHandler(OrbitAIMCAdapter adapter) {
    this.adapter = adapter;
    parameters = new HashMap<>();
  }

  /**
   * Queries OBSW parameters values from the supervisor and set our internal parameters with those
   * values.
   * 
   * @param subscribe True if we want supervisor to push new parameters data, false
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public synchronized UInteger toggleSupervisorParametersSubscription(boolean subscribe) {
    parametersNames = getParametersToEnable();
    if (parametersNames == null) {
      parametersNames = new ArrayList<>();
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


    if (subscribe) {
      if (!openTrainingDataFileStream()) {
        return new UInteger(4);
      }

      // Only if first time, create and register the parameters listener
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
            LOGGER.log(Level.FINE, String.format(
                "Received value %s from supervisor for parameter %s", dataS, parameterName));

            setParameter(parameterName, dataS);
          }
        };

        adapter.getSupervisorSMA().addDataReceivedListener(this.parameterListener);
        LOGGER.log(Level.INFO, "Started fetching parameters from supervisor");
      }
    } else {
      if (!closeTraininDataFileStream()) {
        return new UInteger(5);
      }

      LOGGER.log(Level.INFO, "Stopped fetching parameters from supervisor");
    }

    return null;
  }

  /**
   * Parses the parameters to enable properties and returns the list of OBSW parameter we want to
   * enable the generation in the supervisor.
   *
   * @return The list of parameter identifiers or null if an error occurred.
   */
  private List<String> getParametersToEnable() {
    String content = OrbitAIConf.getinstance().getProperty(OrbitAIConf.PARAMS_TO_ENABLE);
    if (content == null) {
      LOGGER.log(Level.SEVERE, "Error while loading parameters to enable in supervisor");
      return null;
    }

    // parse the line
    List<String> paramNames = new ArrayList<String>();
    for (String paramName : content.split(",")) {
      paramNames.add(paramName);
    }

    if (paramNames.size() <= 0) {
      LOGGER.log(Level.WARNING,
          String.format("Found no parameters to enable in property content %s", content));
    }
    return paramNames;
  }

  /**
   * Sets a parameter with the given value.
   *
   * @param parameterName The parameter name
   * @param value The new value for the parameter
   */
  private void setParameter(String parameterName, String value) {
    parametersLock.lock();
    parameters.put(parameterName, value);
    parametersLock.unlock();
  }

  /**
   * Returns latest parameters values fetched from supervisor at the time of call.
   *
   * @return The map of parameters names and their values
   */
  private Map<String, String> getStampedParametersValues() {
    Map<String, String> parametersValues = new HashMap<>();
    parametersLock.lock();
    for (String parameterName : parametersNames) {
      parameters.computeIfAbsent(parameterName, k -> PARAMS_DEFAULT_VALUE);
      parametersValues.put(parameterName, parameters.get(parameterName));
    }
    parametersLock.unlock();

    return parametersValues;
  }

  /**
   * Returns a formatted time stamp for the time of the call.
   *
   * @return the time stamp formatted as a String
   */
  public static String formatTimestamp(long timestamp) {
    return timestampFormat.format(new Date(timestamp));
  }

  /**
   * Returns a time stamp for the time of the call.
   *
   * @return the time stamp in milliseconds
   */
  public static long getTimestamp() {
    return System.currentTimeMillis();
  }

  /**
   * Opens the training data file stream. Also creates the toGround/ directory if not present
   * already.
   * 
   * @return True if everything went fine, false if an IOException occurred
   */
  private boolean openTrainingDataFileStream() {
    // Create containing directory if needed
    File dataDirectory = Paths.get(OrbitAIApp.TO_GROUND_DIR, DATA_DIR).toFile();
    if (!(dataDirectory.exists() && dataDirectory.isDirectory())) {
      if (!dataDirectory.mkdirs()) {
        LOGGER.log(Level.SEVERE,
            String.format("Couldn't create directory %s", dataDirectory.getPath()));
        return false;
      }
    }

    // Open file stream
    String fileNameExtension = formatTimestamp(getTimestamp()) + ".csv";
    String fileName = "data_" + fileNameExtension;

    if (trainingDataOutputStream == null) {
      try {
        trainingDataOutputStream =
            new FileOutputStream(Paths.get(dataDirectory.getPath(), fileName).toFile(), true);
        // write first line with parameters names
        StringBuffer line = new StringBuffer();
        for (String parameterName : parametersNames) {
          line.append(parameterName);
          line.append(",");
        }
        line.append("\n");
        trainingDataOutputStream.write(line.toString().getBytes());
      } catch (FileNotFoundException e) {
        LOGGER.log(Level.SEVERE, "Couldn't create data file stream", e);
        return false;
      } catch (IOException e) {
        LOGGER.log(Level.SEVERE, "Couldn't write first line to data file stream", e);
        return false;
      }
      return true;
    } else {
      LOGGER.log(Level.SEVERE, "Didn't create data file stream, stream is already created");
      return false;
    }
  }

  /**
   * Closes the training data file stream.
   * 
   * @return True if everything went fine, false if an IOException occurred
   */
  private boolean closeTraininDataFileStream() {
    if (trainingDataOutputStream != null) {
      try {
        trainingDataOutputStream.close();
      } catch (IOException e) {
        LOGGER.log(Level.WARNING, "Error while closing training data file output stream", e);
        return false;
      } finally {
        trainingDataOutputStream = null;
      }
    }
    return true;
  }

  /**
   * Returns parameters values fetched from supervisor at the time of call. Also logs their values
   * in the data handler logs.
   *
   * @return A time stamped map of parameters names and their values.
   */
  public ImmutablePair<Long, Map<String, String>> getDataSet() {
    Map<String, String> dataSet = getStampedParametersValues();
    long timestamp = getTimestamp();

    // format line
    StringBuffer line = new StringBuffer();
    line.append(timestamp);
    line.append(",");
    for (String parameterName : parametersNames) {
      line.append(dataSet.get(parameterName));
      line.append(",");
    }
    line.replace(line.length() - 1, line.length(), "\n");

    // write line
    try {
      trainingDataOutputStream.write(line.toString().getBytes());
    } catch (IOException e) {
      LOGGER.log(Level.WARNING, "Could not write to data file output stream", e);
    }

    return new ImmutablePair<>(timestamp, dataSet);
  }

  /**
   * Returns the label (ON/OFF) associated to a PD6 elevation for the HD camera.
   *
   * @param PD6 the photo-diode 6 (located on -z face of the S/C body) elevation
   * @return the label, 1 = ON, 0 = OFF
   */
  public static int getHDCameraLabel(float PD6) {
    // if elevation, superior than threshold, camera should be OFF
    if (PD6 > PD6_ELEVATION_THRESHOLD_HD_CAM) {
      return 0;
    }

    return 1;
  }
}
