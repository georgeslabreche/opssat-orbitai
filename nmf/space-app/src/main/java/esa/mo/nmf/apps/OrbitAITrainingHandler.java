// (C) 2021 European Space Agency
// European Space Operations Centre
// Darmstadt, Germany

package esa.mo.nmf.apps;

import java.util.logging.Level;
import java.util.logging.Logger;
import org.ccsds.moims.mo.mal.structures.UInteger;

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
   * True if the training handler is stopped (not starting, stopping or training), false otherwise.
   */
  private boolean isStopped;

  /**
   * True if the training handler is currently in the training loop.
   */
  private boolean isLooping;


  public OrbitAITrainingHandler(OrbitAIMCAdapter adapter) {
    this.adapter = adapter;
    isStopped = true;
    isLooping = false;
  }

  /**
   * 
   * TODO startTraining
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public UInteger startTraining() {
    isStopped = false;

    startTrainingLoop();
    return null;
  }

  /**
   * 
   * TODO stopTraining
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public UInteger stopTraining() {
    stopTrainingLoop();

    isStopped = true;
    return null;
  }


  /**
   * 
   * TODO startTrainingLoop
   *
   */
  private void startTrainingLoop() {
    isLooping = true;
    while (isLooping) {
      try {
        LOGGER.log(Level.INFO, "Training");


        Thread.sleep(1000);
      } catch (InterruptedException e) {
      }
    }
  }

  /**
   * 
   * TODO stopTrainingLoop
   *
   */
  private void stopTrainingLoop() {
    isLooping = false;
  }
}
