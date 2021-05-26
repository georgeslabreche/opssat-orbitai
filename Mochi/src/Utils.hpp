#ifndef LOGGING_H_
#define LOGGING_H_

#include <algorithm>
#include <unistd.h>
#include <iomanip>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <sys/stat.h>

#include "Constants.hpp"

using namespace std;

/**
 * Check if file exists.
 */
static inline bool exists(const string& name)
{
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0); 
}

/**
 * Transform a string with separator characters into a vector of substrings.
 */
static inline void str2vector(string str, char sep, vector<string>* pResult)
{
    /* Create string stream from the string. */
    stringstream s_stream(str);
    
    /* Loop. */
    while(s_stream.good())
    {
        string substr;

        /* Get first string delimited by the given separator. */
        getline(s_stream, substr, sep);

        /* Insert substring into vector. */
        pResult->push_back(substr);
    }
}

/**
 * Get timestamp as milliseconds.
 */
static inline string getTimestampMs()
{
    const auto now = chrono::system_clock::now();
    chrono::milliseconds nowMs = chrono::duration_cast<chrono::milliseconds>(
        now.time_since_epoch());

    stringstream nowMsStr;
    nowMsStr << nowMs.count();

    return nowMsStr.str();
}

/**
 * Get a precise timestamp as a string.
 * Taken from: https://gist.github.com/bschlinker/844a88c09dcf7a61f6a8df1e52af7730
 */
static inline string getTimestamp()
{
    chrono::time_point<chrono::system_clock> now = chrono::system_clock::now();
    chrono::milliseconds nowMs = chrono::duration_cast<chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    time_t nowAsTimeT = chrono::system_clock::to_time_t(now);
  
    stringstream nowSs;
    nowSs 
        << std::put_time(std::localtime(&nowAsTimeT), "%Y-%m-%d %T")
        << '.' << std::setfill('0') << std::setw(3) << nowMs.count();

    return nowSs.str();
}


/**
 * Logging a message.
 */
static inline void logMessage(string message, string level)
{
    /* Open the message log file. */
    ofstream ofs;
    ofs.open(LOG_FILEPATH_ORBITAI, std::ios_base::out | std::ios_base::app);

    /* Append the log message into the error log file. */
    ofs << "[" << getTimestamp() << "][" << level << "] " << message << "\n";

    /* Close the log file. */
    ofs.close();
}

/**
 * Logging errors into the application log file.
 */
static inline void logError(string message)
{
    logMessage(message, "ERROR");
}

/**
 * Logging error message prefixed with error code.
 */
static inline void logError(int errorCode, string message)
{
    logError("Error Code " + to_string(errorCode) + ": " + message);
}

/**
 * Logging info into the application log file.
 */
static inline void logInfo(string message)
{
    logMessage(message, "INFO");
}

/**
 * Log the inference results in a CSV file.
 * The pointer to the param names is only required in case the file is created for the first time and a header row is needed.
 */ 
static inline void logInferenceResult(vector<string>* pParamNames, string* pInput, vector<pair<string, int>>* pInferences)
{
    /* Write the header row if the file is being created. */
    bool writeHeader = !exists(LOG_FILEPATH_INFERENCE);

    /* Open the file, append to it if it already exists. */
    ofstream ofs;
    ofs.open(LOG_FILEPATH_INFERENCE, std::ios_base::out | std::ios_base::app);

    /* Write header row. */
    if(writeHeader)
    {
        /* First two columns are the timestamp and the target label. */
        ofs << "timestamp,target,";

        /* Then the param names. */
        for (vector<string>::iterator it = pParamNames->begin(); it != pParamNames->end(); ++it)
        {
            ofs << *it << ",";
        }

        /* And finally the name of the algorithms that made the label inferences/predictions. */
        for (vector<pair<string, int>>::iterator it = pInferences->begin(); it != pInferences->end(); ++it)
        {
            ofs << it->first;

            /* Append comma if it's not the last element of the CSV row that's being built. */
            if(it != pInferences->end() - 1)
            {
                ofs << ",";
            }
        }

        /* End line for the header row. */
        ofs << std::endl;
    }

    /* Write the inference/prediction row. */
    /* The data input needs to be converted from whitespace-separated to comma-separated. */
    vector<string> inputs;
    str2vector(*pInput, ' ', &inputs);

    /* The first two values are the timestamp and the target label. */
    /* The target label is the first element of the data input vector. */
    ofs << getTimestampMs() << "," << std::stoi(inputs.at(0)) << ",";

    /** 
     * The remaining elements of the data input vector are the data inputs themselves.
     * Each data input is prefix with an index and colon, they have to be removed prior to logging.
     *      Before: [1:5.232, 2:4.412, 3:3.123, 4:2.23223]
     *      After:  [  5.232,   4.412,   3.123,   2.23223]
     */
    for (vector<string>::iterator it = inputs.begin()+1; it != inputs.end(); ++it)
    {
        /* Fetch the input value, ignore the index and colon character. */
        vector<string> inputPair;
        str2vector(*it, ':', &inputPair);

        /* Write input value. */
        ofs << inputPair.at(1) << ",";
    }

    /* Then write the inference/prediction results. */
    for (vector<pair<string, int>>::iterator it = pInferences->begin(); it != pInferences->end(); ++it)
    {
        /* Write inference. */
        ofs << it->second;

        /* Append comma if it's not the last element of the CSV row that's being built. */
        if(it != pInferences->end() - 1)
        {
            ofs << ",";
        }
    }

    /* End line for the inference/prediction row. */
    ofs << std::endl;

    /* Close the file. */
    ofs.close();
}

static inline void logTrainingData(vector<string>* pParamNames, string* pInput)
{
    /* Write the header row if the file is being created. */
    bool writeHeader = !exists(LOG_FILEPATH_TRAINING);

    /* Open the file, append to it if it already exists. */
    ofstream ofs;
    ofs.open(LOG_FILEPATH_TRAINING, std::ios_base::out | std::ios_base::app);

    /* Write header row. */
    if(writeHeader)
    {
        /* First two columns are the timestamp and the target label. */
        ofs << "timestamp,target,";

        /* Then the param names. */
        for (vector<string>::iterator it = pParamNames->begin(); it != pParamNames->end(); ++it)
        {
            ofs << *it;

            /* Append comma if it's not the last element of the CSV row that's being built. */
            if(it != pParamNames->end() - 1)
            {
                ofs << ",";
            }
        }

        /* End line for the header row. */
        ofs << std::endl;
    }

    /* Write the training data row. */
    /* The data input needs to be converted from whitespace-separated to comma-separated. */
    vector<string> inputs;
    str2vector(*pInput, ' ', &inputs);

    /* The first two values are the timestamp and the target label. */
    /* The target label is the first element of the data input vector. */
    ofs << getTimestampMs() << "," << std::stoi(inputs.at(0)) << ",";

    /** 
     * The remaining elements of the data input vector are the data inputs themselves.
     * Each data input is prefix with an index and colon, they have to be removed prior to logging.
     *      Before: [1:5.232, 2:4.412, 3:3.123, 4:2.23223]
     *      After:  [  5.232,   4.412,   3.123,   2.23223]
     */
    for (vector<string>::iterator it = inputs.begin()+1; it != inputs.end(); ++it)
    {
        /* Fetch the input value, ignore the index and colon character. */
        vector<string> inputPair;
        str2vector(*it, ':', &inputPair);

        /* Write input value. */
        ofs << inputPair.at(1);

        /* Append comma if it's not the last element of the CSV row that's being built. */
        if(it != inputs.end() - 1)
        {
            ofs << ",";
        }
    }

    ofs << std::endl;

    /* Close the file. */
    ofs.close();
}

#endif // LOGGING_H_