package esa.mo.nmf.apps;

import esa.mo.nmf.MonitorAndControlNMFAdapter;
import esa.mo.nmf.annotations.Parameter;
import esa.mo.nmf.commonmoadapter.SimpleDataReceivedListener;
import esa.mo.nmf.spacemoadapter.SpaceMOApdapterImpl;
import java.io.Serializable;
import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Monitoring and control interface of the OrbitAI space application. Handles parameters and
 * actions.
 */
public class OrbitAIMCAdapter extends MonitorAndControlNMFAdapter {

  private static final Logger LOGGER = Logger.getLogger(OrbitAIMCAdapter.class.getName());

  // ----------------------------------- Parameters -----------------------------------------------

  private static final float PARAMS_DEFAULT_VALUE = 42;

  // -------- RE-EXPOSING parameters consumed from supervisor --------

  // CADC

  @Parameter(description = "I_OM_FB_FI_FB_0 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0880 = PARAMS_DEFAULT_VALUE;

  /*

  @Parameter(description = "I_OM_FB_FI_FB_0 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0881 = PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_OM_FB_FI_FB_0 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0882 = PARAMS_DEFAULT_VALUE;

  // CADC PHOTODIOES parameters

  @Parameter(description = "I_PD1_THETA fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0884 = PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD2_THETA fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0886 = PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD3_THETA fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0888 = PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD4_THETA fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0890 = PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD5_THETA fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = 5, readOnly = true)
  private float CADC0892 = PARAMS_DEFAULT_VALUE;

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
  */

  /**
   * Queries OBSW parameters values from the supervisor and set our internal parameters with those
   * values.
   */
  public void startQueryingSupervisorParameters(SpaceMOApdapterImpl supervisorSMA) {
    // Toggle parameters generation in supervisor
    List<String> parameterNames = new ArrayList<String>();
    for (Field f : this.getClass().getDeclaredFields()) {
      if (f.getName().startsWith("CADC")) {
        parameterNames.add(f.getName());
      }
    }
    supervisorSMA.toggleParametersGeneration(parameterNames, true);

    // Listens for parameters values from supervisor
    supervisorSMA.addDataReceivedListener(new SimpleDataReceivedListener() {
      @Override
      public void onDataReceived(String parameterName, Serializable data) {
        if(data == null) {
          LOGGER.log(Level.WARNING,
              String.format("Received null value for parameter %s", parameterName));
          return;
        }

        String dataS = data.toString();
        try {
          Field internalParameter =
              OrbitAIMCAdapter.this.getClass().getDeclaredField(parameterName);
          internalParameter.setAccessible(true);
          internalParameter.set(OrbitAIMCAdapter.this, Float.parseFloat(dataS));
          internalParameter.setAccessible(false);
        } catch (NoSuchFieldException e) {
          // received parameter that we don't need
          LOGGER.log(Level.WARNING, String
              .format("Received unused value for parameter %s from supervisor", parameterName));
        } catch (SecurityException | IllegalArgumentException | IllegalAccessException e1) {
          LOGGER.log(Level.SEVERE,
              "Error setting internal parameter from supervisor acquired value", e1);
        }
        LOGGER.log(Level.INFO, String.format("Received value %s from supervisor for parameter %s",
            dataS, parameterName));
      }
    });
  }
}
