package esa.mo.nmf.apps;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.ccsds.moims.mo.mal.provider.MALInteraction;
import org.ccsds.moims.mo.mal.structures.UInteger;
import esa.mo.nmf.CloseAppListener;
import esa.mo.nmf.MonitorAndControlNMFAdapter;
import esa.mo.nmf.annotations.Action;
import esa.mo.nmf.annotations.Parameter;
import esa.mo.nmf.nanosatmoconnector.NanoSatMOConnectorImpl;
import esa.mo.nmf.spacemoadapter.SpaceMOApdapterImpl;

/**
 * Monitoring and control interface of the OrbitAI space application. Forwards actions and collects
 * parameters values needed for the training.
 */
public class OrbitAIMCAdapter extends MonitorAndControlNMFAdapter {

  private static final Logger LOGGER = Logger.getLogger(OrbitAIMCAdapter.class.getName());

  public OrbitAIMCAdapter() {
    this.dataHandler = new OrbitAIDataHandler(this);
    this.learningHandler = new OrbitAILearningHandler(this);
  }


  // ----------------------------------- Parameters -----------------------------------------------

  // --- TODO Application's own parameters ---

  @Parameter(description = "Experiment mode read from configuration", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private String oai_mode = "unknown";

  @Parameter(description = "Whether we are fetching data from supervisor", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private boolean oai_isFetchingData = false;

  @Parameter(description = "Whether we are currently learning", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private boolean oai_isLearning = false;

  @Parameter(description = "Interval between 2 iterations read from configuration",
      generationEnabled = true, rawUnit = "seconds",
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private int oai_iterationInterval = -1;

  @Parameter(description = "Number of learning iterations left", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private int oai_iterationsLeft = -1;

  // --- RE-EXPOSING parameters consumed from supervisor ---

  /**
   * Storing supervisor parameters names for convenience.
   */
  public static final List<String> parametersNames = new ArrayList<>(
      Arrays.asList("CADC0884", "CADC0886", "CADC0888", "CADC0890", "CADC0892", "CADC0894",
          "CADC0900", "CADC0901", "CADC0902", "CADC1002", "CADC1003", "CADC1004", "CADC1005"));

  // CADC PHOTODIOES

  @Parameter(description = "I_PD1_THETA fetched from supervisor", generationEnabled = true,
      rawUnit = "radians",
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC0884 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD2_THETA fetched from supervisor", generationEnabled = true,
      rawUnit = "radians",
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC0886 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD3_THETA fetched from supervisor", generationEnabled = true,
      rawUnit = "radians",
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC0888 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD4_THETA fetched from supervisor", generationEnabled = true,
      rawUnit = "radians",
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC0890 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD5_THETA fetched from supervisor", generationEnabled = true,
      rawUnit = "radians",
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC0892 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_PD6_THETA fetched from supervisor", generationEnabled = true,
      rawUnit = "radians",
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC0894 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  // CADC SUN VECTOR (from raw physical sensor)

  @Parameter(description = "I_SS_VEC_0 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC0900 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_SS_VEC_1 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC0901 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "I_SS_VEC_2 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC0902 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  // CADC QUATERNION

  @Parameter(description = "O_Q_FB_FI_EST_0 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC1002 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "O_Q_FB_FI_EST_1 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC1003 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "O_Q_FB_FI_EST_2 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC1004 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;

  @Parameter(description = "O_Q_FB_FI_EST_3 fetched from supervisor", generationEnabled = true,
      reportIntervalSeconds = OrbitAIDataHandler.PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true)
  private float CADC1005 = OrbitAIDataHandler.PARAMS_DEFAULT_VALUE;



  // ----------------------------------- Actions --------------------------------------------------

  /**
   * Class handling actions related to machine learning.
   */
  private OrbitAILearningHandler learningHandler;

  /**
   * Class handling actions related to data.
   */
  private OrbitAIDataHandler dataHandler;

  /**
   * Returns the data handler of the application.
   *
   * @return the data handler
   */
  public OrbitAIDataHandler getDataHandler() {
    return dataHandler;
  }

  @Action(description = "Starts fetching data from the supervisor", stepCount = 1,
      name = "startFetchingData")
  public UInteger startFetchingData(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return dataHandler.toggleSupervisorParametersSubscription(true);
  }

  @Action(description = "Stops fetching data from the supervisor", stepCount = 1,
      name = "stopFetchingData")
  public UInteger stopFetchingData(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return dataHandler.toggleSupervisorParametersSubscription(false);
  }

  @Action(description = "Starts learning depending on the experiment's mode (train, inference ...)",
      stepCount = 1, name = "startLearning")
  public UInteger startLearning(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return learningHandler.startLearning();
  }

  // TODO restart training action (on previous models)

  @Action(description = "Stops learning", stepCount = 1, name = "stopLearning")
  public UInteger stopLearning(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return learningHandler.stopLearning();
  }

  // ----------------------------------- NMF components --------------------------------------------

  /**
   * The application's NMF provider.
   */
  private NanoSatMOConnectorImpl connector;

  /**
   * The application's NMF consumer (consuming supervisor).
   */
  private SpaceMOApdapterImpl supervisorSMA;

  /**
   * Returns the NMF connector, the application's NMF provider.
   * 
   * @return the connector
   */
  public NanoSatMOConnectorImpl getConnector() {
    return connector;
  }

  /**
   * Sets the NMF connector, the application's NMF provider.
   * 
   * @param the connector to set
   */
  public void setConnector(NanoSatMOConnectorImpl connector) {
    this.connector = connector;

    // Define application behavior when closed
    this.connector.setCloseAppListener(new CloseAppListener() {
      @Override
      public Boolean onClose() {
        boolean success = true;
        // Stop fetching data in supervisor
        if (dataHandler.toggleSupervisorParametersSubscription(false) != null) {
          success = false;
        }
        // Stop training models
        if (learningHandler.stopLearning() != null) {
          success = false;
        }
        // Close supervisor consumer connections
        supervisorSMA.closeConnections();

        LOGGER.log(Level.INFO, "Closed application successfully: " + success);
        return success;
      }
    });
  }

  /**
   * Returns the application's NMF consumer (consuming supervisor).
   * 
   * @return the consumer
   */
  public SpaceMOApdapterImpl getSupervisorSMA() {
    return supervisorSMA;
  }

  /**
   * Sets the the application's NMF consumer (consuming supervisor).
   * 
   * @param the consumer to set
   */
  public void setSupervisorSMA(SpaceMOApdapterImpl supervisorSMA) {
    this.supervisorSMA = supervisorSMA;
  }
}
