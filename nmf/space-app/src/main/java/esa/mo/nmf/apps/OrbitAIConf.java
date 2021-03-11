// (C) 2021 European Space Agency
// European Space Operations Centre
// Darmstadt, Germany

package esa.mo.nmf.apps;

import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Holds OrbitAI application configuration constants and helper methods.
 *
 * @author Tanguy Soto
 */
public class OrbitAIConf {

  private static final Logger LOGGER = Logger.getLogger(OrbitAIConf.class.getName());

  /**
   * Singleton instance.
   */
  private static OrbitAIConf instance;

  /**
   * Path to the application configuration file.
   */
  private static final String PROPERTIES_FILE_PATH = "orbit_ai.properties";

  // ========== AVAILABLE PROPERTIES

  /**
   * Experiment mode.
   */
  public static final String MODE = "esa.mo.nmf.apps.OrbitAI.mode";

  /**
   * Number of iterations the experiment should last
   */
  public static final String ITERATIONS = "esa.mo.nmf.apps.OrbitAI.iterations";

  /**
   * Time interval between 2 iterations in seconds
   */
  public static final String INTERVAL = "esa.mo.nmf.apps.OrbitAI.interval";

  /**
   * OBSW parameters for which the publishing will be enabled in NMF supervisor.
   */
  public static final String PARAMS_TO_ENABLE = "esa.mo.nmf.apps.OrbitAI.params_to_enable";

  /**
   * Port at which the Mochi server will be listening.
   */
  public static final String MOCHI_PORT = "esa.mo.nmf.apps.OrbitAI.mochi.port";


  /**
   * Configuration properties holder.
   */
  private Properties properties;

  /**
   * Hide constructor.
   */
  private OrbitAIConf() {}

  /**
   * Returns the Configuration instance of the application.
   *
   * @return the configuration instance.
   */
  public static OrbitAIConf getinstance() {
    if (instance == null) {
      instance = new OrbitAIConf();
    }
    return instance;
  }

  /**
   * Loads the properties from the configuration file located at @v
   *
   */
  public void loadProperties() {
    try (InputStream input = new FileInputStream(PROPERTIES_FILE_PATH)) {
      properties = new Properties();
      properties.load(input);
    } catch (IOException e) {
      LOGGER.log(Level.SEVERE, "Error loading the application configuration properties", e);
    }

    LOGGER.log(Level.INFO,
        String.format("Loaded configuration properties from file %s", PROPERTIES_FILE_PATH));
  }

  /**
   * Searches for the property with the specified key in the application's properties.
   *
   * @param key The property key
   * @return The property or null if the property is not found
   */
  public String getProperty(String key) {
    String property = properties.getProperty(key);
    if (property == null) {
      LOGGER.log(Level.SEVERE,
          String.format("Couldn't find property with key %s, returning null", key));
    }
    return property;
  }
}
