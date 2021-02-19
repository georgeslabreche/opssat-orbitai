package esa.mo.nmf.apps;

import java.util.logging.Level;
import java.util.logging.Logger;
import esa.mo.nmf.nanosatmoconnector.NanoSatMOConnectorImpl;
import esa.mo.nmf.spacemoadapter.SpaceMOApdapterImpl;

/**
 * Main class to start the OrbitAI space application.
 *
 * @author Tanguy Soto
 */
public class OrbitAIApp {

  private static final Logger LOGGER = Logger.getLogger(OrbitAIApp.class.getName());

  // Provider
  private NanoSatMOConnectorImpl connector;
  private OrbitAIMCAdapter adapter;

  // Consumer
  private SpaceMOApdapterImpl supervisorSMA;

  public OrbitAIApp() {
    // Init provider
    connector = new NanoSatMOConnectorImpl();
    adapter = new OrbitAIMCAdapter();
    connector.init(adapter);

    // Init consumer
    supervisorSMA =
        SpaceMOApdapterImpl.forNMFSupervisor(connector.readCentralDirectoryServiceURI());
    adapter.toggleSupervisorParametersSubscription(supervisorSMA, true);

    LOGGER.log(Level.INFO, "OrbitAI initialized.");
  }

  /**
   * Returns the connector.
   * 
   * @return a NanoSatMOConnectorImpl containing the connector of this OrbitAIApp
   */
  public NanoSatMOConnectorImpl getConnector() {
    return connector;
  }

  /**
   * Returns the adapter.
   * 
   * @return a OrbitAIMCAdapter containing the adapter of this OrbitAIApp
   */
  public OrbitAIMCAdapter getAdapter() {
    return adapter;
  }


  /**
   * Main command line entry point.
   *
   * @param args the command line arguments
   * @throws java.lang.Exception If there is an error
   */
  public static void main(final String args[]) throws Exception {
    new OrbitAIApp();
  }
}
