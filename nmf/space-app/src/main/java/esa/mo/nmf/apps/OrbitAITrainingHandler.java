// (C) 2021 European Space Agency
// European Space Operations Centre
// Darmstadt, Germany

package esa.mo.nmf.apps;

import java.io.File;
import java.io.IOException;
import java.net.Socket;
import java.nio.file.Paths;
import java.util.Map;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.apache.commons.io.FileUtils;
import org.ccsds.moims.mo.mal.structures.UInteger;

/**
 * Handles tasks related to models training: start/stop the training, save the models.
 *
 * @author Tanguy Soto
 */
public class OrbitAITrainingHandler {

  private static final Logger LOGGER = Logger.getLogger(OrbitAITrainingHandler.class.getName());

  /**
   * Relative path to the directory that will contain trained models inside the toGround/ folder.
   */
  private static final String EXPORT_MODELS_DIR = "trained_models";

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


  // ----- MOCHI MOCHI ----- //

  private static final String MOCHI_DIR = "/home/tanguy/Documents/code/opssat/opssat-orbitai/Mochi";
  private static final String MOCHI_MODELS_DIR = "models";
  private static final String MOCHI_CMD = "./OrbitAI_Mochi";

  private static final String MOCHI_ADDRESS = "127.0.0.1";
  private static final int MOCHI_PORT = 9999;

  private static final int MOCHI_TRAIN_INTERVAL = 5; // s
  private static final int MOCHI_SAVE_INTERVAL = 10; // s

  private Process mochiProcess;
  private Socket mochiClient;

  private long lastMochiTrainTime;
  private long lastMochiSaveTime;


  // ----- OMC Boost ----- //

  // TODO OMC Boost integration


  public OrbitAITrainingHandler(OrbitAIMCAdapter adapter) {
    this.adapter = adapter;
    trainingThread = null;
  }

  /**
   * Starts the training.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public UInteger startTraining() {
    // MochiMochi
    UInteger errorCode = startAndConnectToMochi();
    if (errorCode != null) {
      return errorCode;
    }

    // training loop
    errorCode = startTrainingLoop();
    if (errorCode != null) {
      return errorCode;
    }

    return null;
  }

  /**
   * Stops the training.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public UInteger stopTraining() {
    // stop training loop
    stopTrainingLoop();

    // stop MochiMochi process
    UInteger errorCode = stopAndDisconnectFromMochi();
    if (errorCode != null) {
      return errorCode;
    }

    // export models
    errorCode = exportTrainedModels();
    if (errorCode != null) {
      return errorCode;
    }

    return null;
  }

  /**
   * Starts the training loop.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  private UInteger startTrainingLoop() {
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
   */
  private void stopTrainingLoop() {
    // Check that we are training
    if (trainingThread == null || trainingRunnable == null || !trainingThread.isAlive()) {
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
    }

    trainingRunnable = null;
    trainingThread = null;
  }

  /**
   * Copies the trained models and their training data to the application's toGround/ directory to
   * be later downlinked.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  private UInteger exportTrainedModels() {
    String timestamp = OrbitAIDataHandler.getTimestamp();

    // Copy Mochi models
    File trainedMochiModelsDir = Paths.get(MOCHI_DIR, MOCHI_MODELS_DIR).toFile();
    File exportTrainedMochiModelsDir =
        Paths.get(OrbitAIApp.TO_GROUND_DIR, EXPORT_MODELS_DIR, "mochi", timestamp).toFile();
    try {
      FileUtils.copyDirectory(trainedMochiModelsDir, exportTrainedMochiModelsDir);
    } catch (IOException e) {
      LOGGER.log(Level.SEVERE, "Error exporting MochiMochi trained models", e);
      return new UInteger(2);
    }

    LOGGER.log(Level.INFO, "Exported trained models to the toGround/ directory");
    return null;
  }

  /**
   * Starts the MochiMochi process and connects to it over TCP
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  private UInteger startAndConnectToMochi() {
    // Check if it is not already running
    if (mochiProcess != null && mochiProcess.isAlive()) {
      LOGGER.log(Level.SEVERE, "Couldn't start MochiMochi, process is already running");
      return new UInteger(10);
    }

    // Start Mochi process
    ProcessBuilder builder = new ProcessBuilder();
    builder.command(MOCHI_CMD);
    builder.directory(new File(MOCHI_DIR));
    try {
      mochiProcess = builder.start();
      LOGGER.log(Level.INFO, "MochiMochi process started");
    } catch (IOException e) {
      LOGGER.log(Level.SEVERE, "Error while starting MochiMochi process", e);
      return new UInteger(11);
    }

    // Check if not already connected to Mochi process
    if (mochiClient != null && !mochiClient.isClosed()) {
      LOGGER.log(Level.SEVERE,
          "Couldn't connect to MochiMochi process, conncetion is already opened");
      return new UInteger(12);
    }

    // Connect to Mochi process
    try {
      Thread.sleep(3000); // give time for Mochi process to initialize
      mochiClient = new Socket(MOCHI_ADDRESS, MOCHI_PORT);
      LOGGER.log(Level.INFO, "Connected to MochiMochi");
    } catch (IOException e) {
      LOGGER.log(Level.SEVERE, "Error while connecting to MochiMochi process", e);
      return new UInteger(13);
    } catch (InterruptedException e) {
      LOGGER.log(Level.SEVERE,
          "Training thread interrupted while waiting for MochiMochi process initialization", e);
    }

    return null;
  }

  /**
   * Stops the MochiMochi process and disconnects from it.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  private UInteger stopAndDisconnectFromMochi() {
    // Check if we are not already disconnected
    if (mochiClient == null || mochiClient.isClosed()) {
      LOGGER.log(Level.WARNING,
          "Didn't disconnect from MochiMochi process, connection was already closed");
      mochiClient = null;
    } else {
      // Disconnect from it
      try {
        mochiClient.close();
        LOGGER.log(Level.INFO, "Disconnected from MochiMochi process");
      } catch (IOException e) {
        LOGGER.log(Level.SEVERE, "Error while disconnecting from MochiMochi process", e);
        return new UInteger(1);
      } finally {
        mochiClient = null;
      }
    }

    // Check that process is running
    if (mochiProcess == null || !mochiProcess.isAlive()) {
      LOGGER.log(Level.WARNING, "Didn't stop MochiMochi process, process is already dead");
    } else {
      // Stop it
      mochiProcess.destroy();
      LOGGER.log(Level.INFO, "MochiMochi process stopped");
    }

    mochiProcess = null;

    // Save models and data

    return null;
  }

  /**
   * The training runnable, running in a separate thread.
   *
   * @author Tanguy Soto
   */
  public class TrainingRunnable implements Runnable {

    // ----- Thread management ----- //

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
          sendMochiTrainCommand();
          sendMochiSaveCommand();

          Thread.sleep(200);
        } catch (InterruptedException e) {
          LOGGER.log(Level.INFO, "Training thread interrupted while sleeping");
        }
      }
      LOGGER.log(Level.INFO, "Training thread stopped");
    }

    /**
     * Sends a save command to the MochiMochi process.
     */
    private void sendMochiSaveCommand() {
      long now = System.currentTimeMillis();
      if (now - lastMochiSaveTime < MOCHI_SAVE_INTERVAL * 1000) {
        return;
      }
      lastMochiSaveTime = now;

      String command = "save";

      if (mochiClient != null && !mochiClient.isClosed()) {
        try {
          mochiClient.getOutputStream().write(command.getBytes());
        } catch (IOException e) {
          LOGGER.log(Level.SEVERE, "Error sending save command to MochiMochi", e);
        }
      }
    }

    /**
     * Sends a train command to the MochiMochi process. The input data come from our data handler
     * and from our data labeler.
     */
    private void sendMochiTrainCommand() {
      long now = System.currentTimeMillis();
      if (now - lastMochiTrainTime < MOCHI_TRAIN_INTERVAL * 1000) {
        return;
      }
      lastMochiTrainTime = now;

      Map<String, Float> dataSet = adapter.getDataHandler().getDataSet();
      float PD3 = dataSet.get("CADC0888");
      float PD6 = dataSet.get("CADC0894");
      int label = OrbitAIDataHandler.getHDCameraLabel(PD6);
      String command = String.format("train %d 1:%f 2:%f", label, PD3, PD6); // TODO clarify format

      if (mochiClient != null && !mochiClient.isClosed()) {
        try {
          mochiClient.getOutputStream().write(command.getBytes());
        } catch (IOException e) {
          LOGGER.log(Level.SEVERE, "Error sending train command to MochiMochi", e);
        }
      }
    }
  }
}
