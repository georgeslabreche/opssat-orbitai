// (C) 2021 European Space Agency
// European Space Operations Centre
// Darmstadt, Germany

package esa.mo.nmf.apps;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Handles tasks related to models training: start/stop the training, save the models.
 *
 * @author Tanguy Soto
 */
public class OrbitAITrainingHandler {

  private static final Logger LOGGER = Logger.getLogger(OrbitAITrainingHandler.class.getName());

  /**
   * M&C interface of the application.
   */
  private final OrbitAIMCAdapter adapter;

  /**
   * True if the application is currently training models.
   */
  private boolean isTraining;


  public OrbitAITrainingHandler(OrbitAIMCAdapter adapter) {
    this.adapter = adapter;
    isTraining = false;
  }

  /**
   * TODO train
   *
   */
  public void train() {
    while (isTraining) {
      try {
        LOGGER.log(Level.INFO, "Training");
        

        Thread.sleep(1000);
      } catch (InterruptedException e) {
      }
    }
  }
}
