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
   * The training thread.
   */
  private Thread trainingThread;

  /**
   * The training runnable, running in trainingThread.
   */
  private TrainingRunnable trainingRunnable;


  public OrbitAITrainingHandler(OrbitAIMCAdapter adapter) {
    this.adapter = adapter;
    trainingThread = null;
  }

  /**
   * Starts the training loop.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public UInteger startTraining() {
    // Check that we are not already training
    if (trainingThread != null && trainingThread.isAlive()) {
      LOGGER.log(Level.SEVERE, "Couldn't start training thread, thread is already running");
      return new UInteger(1);
    }

    // Start the training loop in separate thread
    trainingRunnable = new TrainingRunnable();
    trainingThread = new Thread(trainingRunnable);
    trainingThread.setDaemon(true);
    trainingThread.start();

    return null;
  }

  /**
   * Stops the training loop.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public UInteger stopTraining() {
    // Check that we are indeed training
    if (trainingThread == null || !trainingThread.isAlive()) {
      LOGGER.log(Level.WARNING, "Didn't stop training thread, thread was already stopped");
    } else {
      // Stop the training loop and wait for separate thread to finish
      trainingRunnable.stop();
      trainingThread.interrupt();
      try {
        trainingThread.join();
      } catch (InterruptedException e) {
        LOGGER.log(Level.WARNING, "Current thread interrupted while joining on training thread");
      }

      trainingRunnable = null;
      trainingThread = null;
    }

    return null;
  }


  /**
   * The training runnable, running in a separate thread.
   *
   * @author Tanguy Soto
   */
  public class TrainingRunnable implements Runnable {

    private boolean doStop = false;

    public synchronized void stop() {
      this.doStop = true;
    }

    private synchronized boolean keepRunning() {
      return this.doStop == false;
    }

    /** {@inheritDoc} */
    @Override
    public void run() {
      LOGGER.log(Level.INFO, "Training thread started");
      while (keepRunning()) {
        try {
          LOGGER.log(Level.INFO, "Training");
          Thread.sleep(1000);

        } catch (InterruptedException e) {
          LOGGER.log(Level.INFO, "Training thread interrupted while sleeping");
        }
      }
      LOGGER.log(Level.INFO, "Training thread stopped");
    }
  }
}
