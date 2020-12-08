package esa.mo.nmf.apps;

import esa.mo.nmf.nanosatmoconnector.NanoSatMOConnectorImpl;

/**
 * Main class to start the OrbitAI space application.
 *
 * @author Tanguy Soto
 */
public class OrbitAIApp {
  /**
   * Main command line entry point.
   *
   * @param args the command line arguments
   * @throws java.lang.Exception If there is an error
   */
  public static void main(final String args[]) throws Exception {
    final NanoSatMOConnectorImpl connector = new NanoSatMOConnectorImpl();
    OrbitAIMCAdapter adapter = new OrbitAIMCAdapter(connector);
    connector.init(adapter);
  }
}
