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

  /**
   * Path to the toGround directory of the application.
   */
  public static final String TO_GROUND_DIRECTORY_PATH = "toGround/";


  public OrbitAIApp() {
    // Initialize M&C interface
    OrbitAIMCAdapter adapter = new OrbitAIMCAdapter();

    // Initialize application's NMF provider
    NanoSatMOConnectorImpl connector = new NanoSatMOConnectorImpl();
    connector.init(adapter);

    // Initialize application's NMF consumer (consuming the supervisor)
    SpaceMOApdapterImpl supervisorSMA =
        SpaceMOApdapterImpl.forNMFSupervisor(connector.readCentralDirectoryServiceURI());

    // Once all initialized, pass them to the M&C interface that handles the application's logic
    adapter.setConnector(connector);
    adapter.setSupervisorSMA(supervisorSMA);

    LOGGER.log(Level.INFO, "OrbitAI initialized.");
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
