package esa.mo.nmf.apps;

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

  /**
   * Internal parameters default report interval in seconds.
   */
  public static final float PARAMS_DEFAULT_REPORT_INTERVAL = 5;

  public OrbitAIMCAdapter() {
    this.dataHandler = new OrbitAIDataHandler(this);
    this.learningHandler = new OrbitAILearningHandler(this);
  }


  // ----------------------------------- Parameters -----------------------------------------------

  /*
   * --- TODO Application's own parameters ---
   * 
   * @Parameter(description = "Experiment mode read from configuration", generationEnabled = true,
   * reportIntervalSeconds = PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true) private String
   * oai_mode = "unknown";
   * 
   * @Parameter(description = "Whether we are fetching data from supervisor", generationEnabled =
   * true, reportIntervalSeconds = PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true) private boolean
   * oai_isFetchingData = false;
   * 
   * @Parameter(description = "Whether we are currently learning", generationEnabled = true,
   * reportIntervalSeconds = PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true) private boolean
   * oai_isLearning = false;
   * 
   * @Parameter(description = "Interval between 2 iterations read from configuration",
   * generationEnabled = true, rawUnit = "seconds", reportIntervalSeconds =
   * PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true) private int oai_iterationInterval = -1;
   * 
   * @Parameter(description = "Number of learning iterations left", generationEnabled = true,
   * reportIntervalSeconds = PARAMS_DEFAULT_REPORT_INTERVAL, readOnly = true) private int
   * oai_iterationsLeft = -1;
   * 
   */

  // ----------------------------------- Actions --------------------------------------------------

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

  // @Action(description = "Starts fetching data from the supervisor", stepCount = 1,
  // name = "startFetchingData")
  public UInteger startFetchingData(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return dataHandler.toggleSupervisorParametersSubscription(true);
  }

  // @Action(description = "Stops fetching data from the supervisor", stepCount = 1,
  // name = "stopFetchingData")
  public UInteger stopFetchingData(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return dataHandler.toggleSupervisorParametersSubscription(false);
  }

  // @Action(description = "Starts learning depending on the experiment's mode (train, inference
  // ...)",
  // stepCount = 1, name = "startLearning")
  public UInteger startLearning(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return learningHandler.startLearning();
  }

  // @Action(description = "Stops learning", stepCount = 1, name = "stopLearning")
  public UInteger stopLearning(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return learningHandler.stopLearning(true);
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
        return OrbitAIMCAdapter.this.onClose(true);
      }
    });
  }

  /**
   * Gracefully closes the application.
   *
   * @param requestFromUser Whether request comes from user
   * @return true in case of success, false otherwise
   */
  public boolean onClose(boolean requestFromUser) {
    boolean success = true;
    // Stop fetching data in supervisor
    if (dataHandler.toggleSupervisorParametersSubscription(false) != null) {
      success = false;
    }

    // Stop learning loop
    if (learningHandler.stopLearning(requestFromUser) != null) {
      success = false;
    }

    // Close supervisor consumer connections
    supervisorSMA.closeConnections();

    LOGGER.log(Level.INFO, "Closed application successfully: " + success);

    // if experiment is over
    if (!requestFromUser) {
      System.exit(success ? 0 : 1);
    }

    return success;
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
