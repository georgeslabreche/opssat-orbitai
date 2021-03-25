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
import org.apache.commons.lang3.tuple.ImmutablePair;
import org.ccsds.moims.mo.mal.structures.UInteger;

/**
 * Handles tasks related to the machine learning: start/stop the learning, save the models, execute
 * the inference.
 *
 * @author Tanguy Soto
 */
public class OrbitAILearningHandler {

  private static final Logger LOGGER = Logger.getLogger(OrbitAILearningHandler.class.getName());

  /**
   * Relative path to the directory that will contain trained models inside the toGround/ folder.
   */
  private static final String EXPORT_LEARNING_DIR = "learning";

  /**
   * M&C interface of the application.
   */
  private final OrbitAIMCAdapter adapter;

  /**
   * The learning thread.
   */
  private Thread learningThread;

  /**
   * The learning runnable, running in learningThread.
   */
  private LearningRunnable learningRunnable;

  /**
   * Experiment mode.
   */
  private String mode = OrbitAIConf.TRAIN_MODE;

  /**
   * Time interval between 2 learning iterations in seconds
   */
  private int learningInterval = 5;

  /**
   * Number of remaining iterations for the learning loop
   */
  private int learningIterations = 1000;

  /**
   * Keep in memory the last time we got a valid data set
   */
  private long lastDataSetTimestamp = -1;

  /**
   * Whether or not we check photo diodes values before sending them to learning models.
   */
  private boolean checkPhotodiodesValues = true;

  /**
   * Whether or not we check the validity flags of the photo diodes before sending their values to
   * the learning models.
   */
  private boolean checkValidityFlags = false;



  // ----- MOCHI MOCHI ----- //

  private static final String SAVE_CMD = "save";
  private static final String RESET_CMD = "reset";
  private static final String TRAIN_CMD = "train";
  private static final String INFER_CMD = "infer";
  private static final String LOAD_CMD = "load";
  private static final String EXIT_CMD = "exit";

  private static final String MOCHI_DIR = "bin/Mochi";
  private static final String MOCHI_MODELS_DIR = "models";
  private static final String MOCHI_LOGS_DIR = "logs";

  private static final String MOCHI_CMD = "./OrbitAI_Mochi";

  private static final String MOCHI_ADDRESS = "127.0.0.1";
  private int mochiPort = 9999;

  private Process mochiProcess;
  private Socket mochiClient;


  // ----- OMC Boost ----- //

  // TODO OMC Boost integration


  public OrbitAILearningHandler(OrbitAIMCAdapter adapter) {
    this.adapter = adapter;
    learningThread = null;

    getProperties();
  }

  /**
   * Get required configuration properties from the application configuration.
   */
  private void getProperties() {
    // Mochi server port
    String mochiPortS = OrbitAIConf.getinstance().getProperty(OrbitAIConf.MOCHI_PORT);
    if (mochiPortS == null) {
      LOGGER.log(Level.WARNING, String
          .format("Couldn't get Mochi server port from configuration, %d will be used", mochiPort));
    } else {
      mochiPort = Integer.parseInt(mochiPortS);
    }

    // learning interval
    String learningIntervalS = OrbitAIConf.getinstance().getProperty(OrbitAIConf.INTERVAL);
    if (learningIntervalS == null) {
      LOGGER.log(Level.WARNING, String.format(
          "Couldn't get learning interval from configuration, %d will be used", learningInterval));
    } else {
      learningInterval = Integer.parseInt(learningIntervalS);
    }

    // learning iterations
    String learningIterationsS = OrbitAIConf.getinstance().getProperty(OrbitAIConf.ITERATIONS);
    if (learningIterationsS == null) {
      LOGGER.log(Level.WARNING,
          String.format("Couldn't get learning iterations from configuration, %d will be used",
              learningIterations));
    } else {
      learningIterations = Integer.parseInt(learningIterationsS);
    }

    // experiment mode
    String modeS = OrbitAIConf.getinstance().getProperty(OrbitAIConf.MODE);
    if (modeS == null) {
      LOGGER.log(Level.WARNING,
          String.format("Couldn't get experiment mode from configuration, %s will be used", mode));
    } else {
      mode = modeS;
    }

    // check photo diodes values
    String checkPhotodiodesValuesS =
        OrbitAIConf.getinstance().getProperty(OrbitAIConf.CHECK_PD_VALUES);
    if (checkPhotodiodesValuesS == null) {
      LOGGER.log(Level.WARNING,
          String.format("Couldn't get checkPhotodiodesValues from configuration, %b will be used",
              checkPhotodiodesValues));
    } else {
      checkPhotodiodesValues = Boolean.parseBoolean(checkPhotodiodesValuesS);
    }

    // check photo diodes validity flags
    String checkValidityFlagsS =
        OrbitAIConf.getinstance().getProperty(OrbitAIConf.CHECK_VALIDITY_FLAGS);
    if (checkValidityFlagsS == null) {
      LOGGER.log(Level.WARNING,
          String.format("Couldn't get checkValidityFlags from configuration, %b will be used",
              checkValidityFlags));
    } else {
      checkValidityFlags = Boolean.parseBoolean(checkValidityFlagsS);
    }
  }

  /**
   * Starts the learning.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public synchronized UInteger startLearning() {
    // MochiMochi
    UInteger errorCode = startAndConnectToMochi();
    if (errorCode != null) {
      return errorCode;
    }

    // learning loop
    errorCode = startLearningLoop();
    if (errorCode != null) {
      return errorCode;
    }

    return null;
  }

  /**
   * Stops the learning.
   *
   * @param requestFromUser Whether request comes from user
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  public synchronized UInteger stopLearning(boolean requestFromUser) {
    // stop learning loop
    stopLearningLoop(requestFromUser);


    // stop MochiMochi process
    UInteger errorCode = stopAndDisconnectFromMochi();
    if (errorCode != null) {
      return errorCode;
    }

    // export learning data
    errorCode = exportLearningData();
    if (errorCode != null) {
      return errorCode;
    }

    return null;
  }

  /**
   * Starts the learning loop.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  private UInteger startLearningLoop() {
    // Check that we are not already learning
    if (learningThread != null && learningThread.isAlive()) {
      LOGGER.log(Level.SEVERE, "Couldn't start learning thread, thread is already running");
      return new UInteger(1);
    }

    // Start the learning loop in separate thread
    learningRunnable = new LearningRunnable();
    learningThread = new Thread(learningRunnable);
    learningThread.setDaemon(true);
    learningThread.start();

    return null;
  }

  /**
   * Stops the learning loop.
   *
   * @param requestFromUser Whether request comes from user
   */
  private void stopLearningLoop(boolean requestFromUser) {
    if (requestFromUser) {
      // Check that we are learning
      if (learningThread == null || learningRunnable == null || !learningThread.isAlive()) {
        LOGGER.log(Level.WARNING, "Didn't stop learning thread, thread was already stopped");
      } else {
        // Stop the learning loop and wait for separate thread to finish
        learningRunnable.stop();
        learningThread.interrupt();
        try {
          learningThread.join();
        } catch (InterruptedException e) {
          LOGGER.log(Level.WARNING, "Current thread interrupted while joining on learning thread");
        }
      }
    }

    learningRunnable = null;
    learningThread = null;
  }

  /**
   * Copies the learning data (models, logs, inferences) to the application's toGround/ directory to
   * be later downlinked.
   *
   * @return null if it was successful. If not null, then the returned value holds the error number
   */
  private UInteger exportLearningData() {
    long timestamp = lastDataSetTimestamp;
    if (timestamp == -1) {
      timestamp = OrbitAIDataHandler.getTimestamp();
    }
    String timestampFormatted = OrbitAIDataHandler.formatTimestamp(timestamp);

    // Copy Mochi models and logs
    File trainedMochiModelsDir = Paths.get(MOCHI_DIR, MOCHI_MODELS_DIR).toFile();
    File mochiLogsDir = Paths.get(MOCHI_DIR, MOCHI_LOGS_DIR).toFile();
    File exportTrainedMochiModelsDir = Paths.get(OrbitAIApp.TO_GROUND_DIR, EXPORT_LEARNING_DIR,
        "mochi-" + timestampFormatted, MOCHI_MODELS_DIR).toFile();
    File exportMochiLogsDir = Paths.get(OrbitAIApp.TO_GROUND_DIR, EXPORT_LEARNING_DIR,
        "mochi-" + timestampFormatted, MOCHI_LOGS_DIR).toFile();
    try {
      FileUtils.copyDirectory(trainedMochiModelsDir, exportTrainedMochiModelsDir);
      FileUtils.copyDirectory(mochiLogsDir, exportMochiLogsDir);
    } catch (IOException e) {
      LOGGER.log(Level.SEVERE, "Error exporting MochiMochi trained models and logs", e);
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
    String[] command = new String[] {MOCHI_CMD, "" + mochiPort};
    builder.command(command);
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
      mochiClient = new Socket(MOCHI_ADDRESS, mochiPort);
      LOGGER.log(Level.INFO, "Connected to MochiMochi");
    } catch (IOException e) {
      LOGGER.log(Level.SEVERE, "Error while connecting to MochiMochi process", e);
      return new UInteger(13);
    } catch (InterruptedException e) {
      LOGGER.log(Level.SEVERE,
          "learning thread interrupted while waiting for MochiMochi process initialization", e);
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
    if (mochiProcess != null && mochiProcess.isAlive()) {
      // Stop it
      mochiProcess.destroy();
      LOGGER.log(Level.WARNING,
          "MochiMochi process had to be stopped, \"exit\" command was probably not received");
    }

    mochiProcess = null;

    return null;
  }

  /**
   * The learning runnable, running in a separate thread.
   *
   * @author Tanguy Soto
   */
  private class LearningRunnable implements Runnable {

    // ----- Thread management ----- //

    private boolean doStop = false;

    public synchronized void stop() {
      this.doStop = true;
    }

    public synchronized boolean stopRequested() {
      return this.doStop;
    }

    private synchronized boolean keepRunning() {
      return !this.doStop && learningIterations > 0;
    }

    /** {@inheritDoc} */
    @Override
    public void run() {
      LOGGER.log(Level.INFO, "Learning thread started");

      // prepare
      if (mode.equals(OrbitAIConf.TRAIN_MODE)) {
        sendMochiCommand(RESET_CMD);
      } else {
        sendMochiCommand(LOAD_CMD);
      }

      // start "learning" loop
      while (keepRunning()) {
        try {
          sendMochiLearnCommand();
          learningIterations--;

          Thread.sleep(learningInterval * 1000);
        } catch (InterruptedException e) {
          LOGGER.log(Level.INFO, "Learning thread interrupted between two iterations");
        }
      }

      // finish
      sendMochiCommand(SAVE_CMD);
      myWait(500);
      sendMochiCommand(EXIT_CMD);
      myWait(500);

      // loop finished only because experiment is over
      if (!stopRequested()) {
        adapter.onClose(false);
      }

      LOGGER.log(Level.INFO, "Learning thread stopped");
    }

    /**
     * Causes the currently executing thread to sleep (temporarily cease execution) for the
     * specified number of milliseconds inside a try-catch for InterruptedException.
     *
     * @param millist he length of time to sleep in milliseconds
     */
    private void myWait(long millis) {
      try {
        Thread.sleep(millis);
      } catch (InterruptedException e) {
      }
    }

    /**
     * Sends a command to the MochiMochi process.
     * 
     * @param command The command
     */
    private void sendMochiCommand(String command) {
      if (command == null) {
        LOGGER.log(Level.WARNING, "Trying to send null command to MochiMochi process.");
        return;
      }

      if (mochiClient != null && !mochiClient.isClosed()) {
        try {
          mochiClient.getOutputStream().write(command.getBytes());
          LOGGER.log(Level.INFO, String.format("Send command to MochiMochi: %s", command));
        } catch (IOException e) {
          LOGGER.log(Level.SEVERE, String.format("Error sending %s command to MochiMochi", command),
              e);
        }
      }
    }

    /**
     * Sends a learn command ({@value #TRAIN_CMD} or {@value #INFER_CMD}) to the MochiMochi process.
     * The input data come from our data handler and from our data labeler.
     */
    private void sendMochiLearnCommand() {
      // get data
      ImmutablePair<Long, Map<String, String>> stampedDataSet =
          adapter.getDataHandler().getDataSet();
      Map<String, String> dataSet = stampedDataSet.getValue();

      // get diodes related data
      String[] PD = new String[7];
      PD[1] = dataSet.get(OrbitAIDataHandler.PD1);
      PD[2] = dataSet.get(OrbitAIDataHandler.PD2);
      PD[3] = dataSet.get(OrbitAIDataHandler.PD3);
      PD[4] = dataSet.get(OrbitAIDataHandler.PD4);
      PD[5] = dataSet.get(OrbitAIDataHandler.PD5);
      PD[6] = dataSet.get(OrbitAIDataHandler.PD6);
      String[] PD_FLAGS = new String[7];
      PD_FLAGS[1] = dataSet.get(OrbitAIDataHandler.PD1_FLAG);
      PD_FLAGS[2] = dataSet.get(OrbitAIDataHandler.PD2_FLAG);
      PD_FLAGS[3] = dataSet.get(OrbitAIDataHandler.PD3_FLAG);
      PD_FLAGS[4] = dataSet.get(OrbitAIDataHandler.PD4_FLAG);
      PD_FLAGS[5] = dataSet.get(OrbitAIDataHandler.PD5_FLAG);
      PD_FLAGS[6] = dataSet.get(OrbitAIDataHandler.PD6_FLAG);

      // check diodes values
      float[] PDf = checkPhotodiodesValues(PD);
      if (PDf == null) {
        return;
      }

      // check diodes validity flags
      if (!checkPhotodiodesValidityFlags(PD_FLAGS)) {
        return;
      }

      // all correct, we send the Mochi command
      int label = OrbitAIDataHandler.getHDCameraLabel(PDf[6]);
      long timestamp = stampedDataSet.getKey();
      lastDataSetTimestamp = timestamp;
      String subCommand = mode.equals(OrbitAIConf.INFERENCE_MODE) ? INFER_CMD : TRAIN_CMD;

      String command = String.format("%s %d %.2f %.2f %.2f %.2f %.2f %.2f %d\n", subCommand, label,
          PDf[1], PDf[2], PDf[3], PDf[4], PDf[5], PDf[6], timestamp);

      sendMochiCommand(command);
    }
  }


  /**
   * Checks the photo diodes values before sending them to the learning models. We carefully parse
   * them to float and then verify if values are in expected range if enabled.
   * 
   * @param PD The photo diodes values (index 0 is unused).
   * @return the photo diodes values or null if any of them is invalid
   */
  private float[] checkPhotodiodesValues(String[] PD) {
    float[] PDf = new float[7];

    for (int i = 1; i < PD.length; i++) {
      // parse to float
      try {
        PDf[i] = Float.parseFloat(PD[i]);
      } catch (NumberFormatException | NullPointerException e) {
        return null;
      }

      // if enabled, check value is in range
      if (checkPhotodiodesValues) {
        if (PDf[i] < 0 || PDf[i] > Math.PI / 2.0) {
          return null;
        }
      }
    }

    return PDf;
  }

  /**
   * @param PD_FLAGS the flags (index 0 is unused)
   * @return true if the validity flags are all equal to "1" or if the check of validity flags is
   *         disabled.
   */
  private boolean checkPhotodiodesValidityFlags(String[] PD_FLAGS) {
    // no need to check
    if (!checkValidityFlags) {
      return true;
    }

    // we check the flags
    for (int i = 1; i < PD_FLAGS.length; i++) {
      if (!"true".equals(PD_FLAGS[i])) {
        return false;
      }
    }
    return true;
  }
}
