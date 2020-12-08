package esa.mo.nmf.apps;

import esa.mo.nmf.MonitorAndControlNMFAdapter;
import esa.mo.nmf.NMFInterface;
import esa.mo.nmf.annotations.Parameter;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Monitoring and control interface of the OrbitAI space application. Handles parameters and
 * actions.
 */
public class OrbitAIMCAdapter extends MonitorAndControlNMFAdapter {

  private static final Logger LOGGER = Logger.getLogger(OrbitAIMCAdapter.class.getName());

  private NMFInterface connector;

  // ----------------------------------- Parameters -----------------------------------------------

  @Parameter(description = "are we exposed to the sun (test parameter)", generationEnabled = true,
      reportIntervalSeconds = 2)
  private boolean isExposedToSun = false;


  public OrbitAIMCAdapter(final NMFInterface connector) {
    this.connector = connector;
    LOGGER.log(Level.INFO, "OrbitAI space application initialized.");
  }

  public NMFInterface getConnector() {
    return connector;
  }
}
