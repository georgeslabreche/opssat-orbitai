package esa.mo.nmf.apps;

import java.util.logging.Level;
import java.util.logging.Logger;
import esa.mo.helpertools.misc.Const;
import esa.mo.nmf.nanosatmoconnector.NanoSatMOConnectorImpl;

/**
 * Main class to start the OrbitAI space application.
 *
 * @author Tanguy Soto
 */
public class OrbitAIApp {

  private static final Logger LOGGER = Logger.getLogger(OrbitAIApp.class.getName());

  // Providers
  private NanoSatMOConnectorImpl connector;
  private OrbitAIMCAdapter adapter;

  public OrbitAIApp() {
    //System.setProperty(Const.CENTRAL_DIRECTORY_URI_PROPERTY,
    //    "maltcp://10.0.2.15:1024/nanosat-mo-supervisor-Directory");

    connector = new NanoSatMOConnectorImpl();
    adapter = new OrbitAIMCAdapter(this);
    connector.init(adapter);

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
    OrbitAIApp app = new OrbitAIApp();
  }
}
