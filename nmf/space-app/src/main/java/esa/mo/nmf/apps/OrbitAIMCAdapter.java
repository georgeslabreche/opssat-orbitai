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

  public OrbitAIMCAdapter() {
    this.dataHandler = new OrbitAIDataHandler(this);
    this.trainingHandler = new OrbitAITrainingHandler(this);
  }


  // ----------------------------------- Parameters -----------------------------------------------

  /**
   * Internal parameters default value.
   */
  private static final float PARAMS_DEFAULT_VALUE = 42;

  // --- RE-EXPOSING parameters consumed from supervisor ---

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


  // ----------------------------------- Actions --------------------------------------------------

  /**
   * Class handling actions related to training the models.
   */
  private OrbitAITrainingHandler trainingHandler;

  /**
   * Class handling actions related to data.
   */
  private OrbitAIDataHandler dataHandler;

  @Action(description = "Starts fetching training data from the supervisor", stepCount = 1,
      name = "startFetchingData")
  public UInteger startFetchingData(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return dataHandler.toggleSupervisorParametersSubscription(true);
  }

  @Action(description = "Stops fetching training data from the supervisor", stepCount = 1,
      name = "stopFetchingData")
  public UInteger stopFetchingData(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return dataHandler.toggleSupervisorParametersSubscription(false);
  }

  @Action(description = "Starts training models, even if we are currently not fetching data",
      stepCount = 1, name = "startTraining")
  public UInteger startTraining(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return trainingHandler.startTraining();
  }

  @Action(description = "Stops training models", stepCount = 1, name = "stopTraining")
  public UInteger stopTraining(Long actionInstanceObjId, boolean reportProgress,
      MALInteraction interaction) {
    return trainingHandler.stopTraining();
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
        if (trainingHandler.stopTraining() != null) {
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
