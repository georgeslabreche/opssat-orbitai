#ifndef LOGGING_H_
#define LOGGING_H_

#include <unistd.h>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>

#include "Constants.hpp"

using namespace std;

/**
 * Get a precise timestamp as a string.
 * Taken from: https://gist.github.com/bschlinker/844a88c09dcf7a61f6a8df1e52af7730
 */
static inline string getTimestamp()
{
  const auto now = std::chrono::system_clock::now();
  const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
  const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch()) % 1000;

  stringstream nowSs;
  nowSs
      << std::put_time(std::localtime(&nowAsTimeT), "%Y-%m-%d %T")
      << '.' << std::setfill('0') << std::setw(3) << nowMs.count();

  return nowSs.str();
}

/**
 * Logging a message.
 */
static inline void log(string message, string level)
{
    /* Open the error log file. */
    ofstream logFile;
    logFile.open(LOG_FILEPATH_ORBITAI, std::ios_base::out | std::ios_base::app);

    /* Append the log message into the error log file. */
    logFile << "[" << getTimestamp() << "][" << level << "] " << message << "\n";

    /* Close the log file. */
    logFile.close();
}

/**
 * Logging errors into the application log file.
 */
static inline void logError(string message)
{
    log(message, "ERROR");
}

/**
 * Logging info into the application log file.
 */
static inline void logInfo(string message)
{
    log(message, "INFO");
}

#endif // LOGGING_H_