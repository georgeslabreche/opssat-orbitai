/*
 * ---------------------------------------------------------------------------- Copyright (C) 2015
 * European Space Agency European Space Operations Centre Darmstadt Germany
 * ---------------------------------------------------------------------------- System : ESA NanoSat
 * MO Framework ----------------------------------------------------------------------------
 * Licensed under the European Space Agency Public License, Version 2.0 You may not use this file
 * except in compliance with the License.
 *
 * Except as expressly set forth in this License, the Software is provided to You on an "as is"
 * basis and without warranties of any kind, including without limitation merchantability, fitness
 * for a particular purpose, absence of defects or errors, accuracy or non-infringement of
 * intellectual property rights.
 * 
 * See the License for the specific language governing permissions and limitations under the
 * License. ----------------------------------------------------------------------------
 */
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
