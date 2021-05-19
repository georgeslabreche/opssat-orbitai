/**
 * TODO: Refactoring
 * - Create a Logging class and move all logging logic inside of it.
 * - Create a SocketServer class and move all socker server logic inside of it.
 * - Create a MochiFacade class and move all calls to the Mochi logic inside of it.
 */

#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit()
#include <iostream> // For cout
#include <fstream>
#include <unistd.h> // For read
#include <dirent.h>

#include <sys/types.h> // For mkdkir
#include <sys/stat.h> // For mkdkir

#include <chrono>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctime>

// For machine learning.
#include <mochimochi/binary_classifier.hpp>
#include <mochimochi/utility.hpp>
#include <mochimochi/classifier/factory/binary_oml_factory.hpp>

#include "PropertiesParser.hpp"
#include "HyperParameters.hpp"

// TODO: Figure out if this is enough or too much.
#define COMMAND_BUFFER_LENGTH                                 10000

#define DIR_PATH_LOGS                                         "logs"
#define DIR_PATH_MODELS                                     "models"

#define LOG_FILEPATH_TRAINING                   "logs/training.csv"
#define LOG_FILEPATH_INFERENCE                 "logs/inference.csv"
#define LOG_FILEPATH_ORBITAI                     "logs/orbitai.log"

/* Error codes. */
#define ERROR_SUCCESS                                             0
#define ERROR_UNKNOWN                                             1
#define ERROR_PARSE_ARGS                                          2
#define ERROR_INVALID_PORT_NUM                                    3
#define ERROR_CREATE_SOCKET                                       4
#define ERROR_BIND_PORT                                           5
#define ERROR_LISTEN_SOCKET                                       6
#define ERROR_GRAB_CONNECTION                                     7

/* Commands. */
#define COMMAND_RESET                                       "reset"
#define COMMAND_RESET_LENGTH                                      5
#define COMMAND_SAVE                                         "save"
#define COMMAND_SAVE_LENGTH                                       4
#define COMMAND_EXIT                                         "exit"
#define COMMAND_EXIT_LENGTH                                       4

using namespace std;

/* Direcotry paths */
string gDirPathModels(DIR_PATH_MODELS);

/* Modes */
enum class Mode {
    trainNew = 0,
    trainContinue = 1,
    infer = 2
};

/* Flag to keep track if models were loaded or not. */
int gModelsLoadedFlag = 0;

/**
 * Get a precise timestamp as a string.
 */
string getTimestamp();

/**
 * Logging into log file.
 */
static inline void log(string message, string level);

/**
 * Logging errors into the log file.
 */
void logError(string message);

/**
 * Logging info into the log file.
 */
void logInfo(string message);

/**
 * Create the enabled algorithm via the Factory Pattern implemented in the MochiMochi library.
 */
void initAlgorithms(int dim, PropertiesParser *pPropParser, map<string, vector<string>> *pHpMap, map<string, BinaryOMLCreator*> *pBomlCreatorMap)
{
    for(map<string, vector<string>>::iterator it=pHpMap->begin(); it!=pHpMap->end(); ++it)
    {
        /* The name of the online ML algorithm. */
        string algorithmName = it->first;

        /* Check if the properties file indicates that this algorithm should be used. */
        int enable = pPropParser->getProperty<int>(algorithmName);
        if(enable == 1)
        {
            if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_ADAGRAD_RDA) == 0)
            {
                /* Get hyperparameter values. */
                const double eta = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const double lambda = pPropParser->getHyperParameterProperty<double>(algorithmName,it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryADAGRADRDACreator(dim, eta, lambda)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_ADAM) == 0)
            {
                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryADAMCreator(dim)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_AROW) == 0)
            {
                /* Get hyperparameter values. */
                const double r = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryAROWCreator(dim, r)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_NHERD) == 0)
            {
                /* Get hyperparameter values. */
                const double c = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const int diagonal = pPropParser->getHyperParameterProperty<int>(algorithmName,it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryNHERDCreator(dim, c, diagonal)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_PA) == 0)
            {
                /* Get hyperparameter values. */
                const double c = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const int select = pPropParser->getHyperParameterProperty<int>(algorithmName,it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinaryPACreator(dim, c, select)));
            }
            else if(algorithmName.compare(HyperParameters::ALGORITHM_NAME_SCW) == 0)
            {
                /* Get hyperparameter values. */
                const double c = pPropParser->getHyperParameterProperty<double>(algorithmName, it->second.at(0));
                const double eta = pPropParser->getHyperParameterProperty<double>(algorithmName,it->second.at(1));

                /* Instanciate the online ML algorithm class into an object and put it in the algorithm map. */ 
                pBomlCreatorMap->insert(pair<string, BinaryOMLCreator*>(algorithmName, new BinarySCWCreator(dim, c, eta)));
            }
        }
    }
}

/**
 * Initialize the socket server. 
 */
void initSocketServer(int portNumber, int *pSockfd, int *pConnection)
{
    // Create a socket (IPv4, TCP)
    *pSockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (*pSockfd == -1)
    {
        // Log error.
        logError("Failed to create socket. errno: " + to_string(errno));

        // Exit with error code.
        exit(ERROR_CREATE_SOCKET);
    }

    // Listen to port on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;

    // Invoking htons is necessary to convert a number to network byte order.
    // FIXME: Read port num from properties file.
    sockaddr.sin_port = htons(portNumber);

    // Bind to the given port.
    if (bind(*pSockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
    {
        // Log error.
        logError("Failed to bind to port " + to_string(portNumber) + ". errno: " + to_string(errno));

        // Exit with error code.
        exit(ERROR_BIND_PORT);
    }

    // Start listening. Hold at most 1 connection in the queue
    if (listen(*pSockfd, 1) < 0)
    {
        // Log error.
        logError("Failed to listen on socket. errno: " + to_string(errno));

        // Exit with error code.
        exit(ERROR_LISTEN_SOCKET);
    }

    // Grab a connection from the queue
    auto addrlen = sizeof(sockaddr);
    *pConnection = accept(*pSockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);

    if (*pConnection < 0)
    {
        // Log error.
        logError("Failed to grab connection. errno: " + to_string(errno));

        // Exit with error code.
        exit(ERROR_GRAB_CONNECTION);
    }
}

/**
 * Close server socket and connection.
 */
void shutdownSocketServer(int *pSockfd, int *pConnection)
{
    /* Close the connection */
    if(*pConnection)
    {
        close(*pConnection);
    }

    /* Close the socket. */
    if(*pSockfd)
    {
        close(*pSockfd);
    }
}

/**
 * Delete all model and log files.
 */
void reset()
{
    /* Delete all model files. */
    DIR *dir;
    struct dirent *ent;

    if((dir = opendir(gDirPathModels.c_str())) != NULL)
    {   
        /* Loop through all the files inside the models directory. */
        while((ent = readdir(dir)) != NULL)
        {
            /* Only process regular image files */
            if(ent->d_type == DT_REG)
            {
                string filname(gDirPathModels + "/" + string(ent->d_name));
                remove(filname.c_str());
            }
        }
    }

    /* Delete all log files. */
    remove(LOG_FILEPATH_TRAINING);
    remove(LOG_FILEPATH_INFERENCE);
    remove(LOG_FILEPATH_ORBITAI);
}

/**
 * Train and save all the models with the given input.
 */
void trainAndSaveModels(map<string, BinaryOMLCreator*> *pBomlCreatorMap, string *pTrainingInput, size_t dim)
{
    for(map<string, BinaryOMLCreator*>::iterator it=pBomlCreatorMap->begin(); it!=pBomlCreatorMap->end(); ++it)
    {
        it->second->trainAndSave(pTrainingInput, dim, gDirPathModels + "/" + it->second->name());
    }
}

/**
 * Use the trained models to infer/predict the label for the given input.
 */
void infer(map<string, BinaryOMLCreator*> *pBomlCreatorMap, string *pTrainingInput, size_t dim, map<string, int> *pPredictions)
{
    for(map<string, BinaryOMLCreator*>::iterator it=pBomlCreatorMap->begin(); it!=pBomlCreatorMap->end(); ++it)
    {
        // TODO: insert prediction result into map
        it->second->infer(pTrainingInput, dim);
    }
}


/**
 * Load all the models for the enabled online ML algorithms.
 */
void loadModels(map<string, BinaryOMLCreator*> *pBomlCreatorMap)
{
    for(map<string, BinaryOMLCreator*>::iterator it=pBomlCreatorMap->begin(); it!=pBomlCreatorMap->end(); ++it)
    {
        // TODO: Handle missing file exception (just skip and log).
        it->second->load(gDirPathModels + "/" + it->second->name());
    }
}

/**
 * Save all the models for the enabled online ML algorithms.
 */
void saveModels(map<string, BinaryOMLCreator*> *pBomlCreatorMap)
{
    for(map<string, BinaryOMLCreator*>::iterator it=pBomlCreatorMap->begin(); it!=pBomlCreatorMap->end(); ++it)
    {
        it->second->save(gDirPathModels + "/" + it->second->name());
    }
}

/**
 * Process the received command.
 */
int processReceivedCommand(int mode, int dim, string *pReceivedCommand, map<string, BinaryOMLCreator*> *pBomlCreatorMap)
{
    if(pReceivedCommand->compare(0, COMMAND_RESET_LENGTH, COMMAND_RESET) == 0)
    {
        /* Delete all model and log files. */
        reset();
    }
    else if(pReceivedCommand->compare(0, COMMAND_SAVE_LENGTH, COMMAND_SAVE) == 0)
    {
        /* Save all models for the enabled algorithms. */
        saveModels(pBomlCreatorMap);
    }
    else if(pReceivedCommand->compare(0, COMMAND_EXIT_LENGTH, COMMAND_EXIT) == 0)
    {
        /* Exit the server loop. */
        return 1;
    }
    else
    {
        /**
         * Entering this else block means that the receive command is either for training or inference.
         * There are three possible modes:
         * 
         *  Mode 0: New Training.
         *  Mode 1: Continue Training.
         *  Mode 2: Inference.
         * 
         * Command structure:
         * - First four characters is the total length of the received message.
         * - The rest is the input string sent as a parameter to the online ML update or predict/infer functions.
         * 
         * E.g. commands:
         * 
         * 0041 +1 1:1.232 2:2.412 3:2.123 4:5.23223
         * 0031 -1 1:1.232 2:2.412 3:2.123
         * 
         * Note that the commands do not start with a command name for which operation mode to execute.
         * This is because we can just determine this from reading the Properties file.
         * 
         */ 

        /* First four characters is the total length of the received message. */
        int messageLength = std::stoi(pReceivedCommand->substr(0, 4));

        /* The rest is the actual training or inference input. */
        string input = pReceivedCommand->substr(5, messageLength-5);

        switch(mode)
        {
            case static_cast<int>(Mode::trainNew):
                /* Train and save models. */
                trainAndSaveModels(pBomlCreatorMap, &input, dim);
                break;

            case static_cast<int>(Mode::trainContinue):

                /* Load models if they weren't loaded already. */
                if(gModelsLoadedFlag == 0)
                {
                    /* Load all the models. */
                    loadModels(pBomlCreatorMap);

                    /* Mark models as having been loaded. */
                    gModelsLoadedFlag = 1;
                }

                /* Train and save models. */
                trainAndSaveModels(pBomlCreatorMap, &input, dim);

                break;

            case static_cast<int>(Mode::infer):
                /* Load models if they weren't loaded already. */
                if(gModelsLoadedFlag == 0)
                {
                    /* Load all the models. */
                    loadModels(pBomlCreatorMap);

                    /* Mark models as having been loaded. */
                    gModelsLoadedFlag = 1;
                }

                map<string, int> predictions;
                infer(pBomlCreatorMap, &input, dim, &predictions);

                // TODO: Log prediction results.

                break;
        }
    }

    return 0;
}

int main(int argc, char *argv[])
{
    /* The map that will contain points to the online ML algorithm class instances. */
    map<string, BinaryOMLCreator*> bomlCreatorMap;

    try
    {
        /* Create models and logs directory if they do not exist. */
        mkdir(DIR_PATH_LOGS, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        mkdir(DIR_PATH_MODELS, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        /* Load the properties file and parse it into a map. */
        PropertiesParser propParser(argv[1]);

        /* The socket serve's port number. */
        const int portNumber = propParser.getPortNumber();

        /* Get mode: 0 - new training, 1 - continue training, or 2 - inference. */
        const int mode = propParser.getMode();

        /* The input dimension. */
        const size_t dim = propParser.getInputDimension();

        /* Convenience class that gives us a map containg a list of hyperparameter names for each online ML algorithm. */
        HyperParameters hyperParams;
        map<string, vector<string>> hpMap = hyperParams.getHyperParamsMap();

        
        /* Init the online ML algorithms. */
        initAlgorithms(dim, &propParser, &hpMap, &bomlCreatorMap);

        /* The buffer for received commands. */
        char buffer[COMMAND_BUFFER_LENGTH];

        /* Init Socket Server. */
        int connection;
        int sockfd;

        initSocketServer(portNumber, &sockfd, &connection);

        // Read from the connection
        while(1)
        {
            // Read received command.
            auto bytesRead = read(connection, buffer, COMMAND_BUFFER_LENGTH);

            // Get string representation of the command.
            string receivedCmd(buffer, bytesRead);

            // Print out received command.
            //std::cout << "Received: " << receivedCmd << std::endl;

            /* Process the received command and break out the server loop if it's an exit command. */
            if(processReceivedCommand(mode, dim, &receivedCmd, &bomlCreatorMap) == 1)
            {
                break;
            }
        }

        /* Close connection and socket. */
        shutdownSocketServer(&sockfd, &connection);

        /* Exit program. */
        exit(ERROR_SUCCESS);
    }
    catch(const std::exception& e)
    {
        // Log error.
        std::ostringstream oss;
        oss << "Error parsing arguments: " << e.what();
        logError(oss.str());
        
        // Exit program with error code.
        exit(ERROR_UNKNOWN);
    }

    // TODO: Handle exception here.
    /* Destroy the BinaryOMLCreator pointers in the Creator map. */
    for(map<string, BinaryOMLCreator*>::iterator it=bomlCreatorMap.begin(); it!=bomlCreatorMap.end(); ++it)
    {
        delete it->second;
        bomlCreatorMap.erase(it);
    }
}


/**
 * Get a precise timestamp as a string.
 * Taken from: https://gist.github.com/bschlinker/844a88c09dcf7a61f6a8df1e52af7730
 */
std::string getTimestamp() 
{
  const auto now = std::chrono::system_clock::now();
  const auto nowAsTimeT = std::chrono::system_clock::to_time_t(now);
  const auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
      now.time_since_epoch()) % 1000;

  std::stringstream nowSs;
  nowSs
      << std::put_time(std::localtime(&nowAsTimeT), "%Y-%m-%d %T")
      << '.' << std::setfill('0') << std::setw(3) << nowMs.count();

  return nowSs.str();
}

/**
 * Logging a message.
 */
static inline void log(std::string message, std::string level)
{
    /* Print out error message. */
    std::cout << message << std::endl;
    
    /* Open the error log file. */
    std::ofstream logFile;
    logFile.open(LOG_FILEPATH_ORBITAI, std::ios_base::out | std::ios_base::app);

    /* Append the log message into the error log file. */
    logFile << "[" << getTimestamp() << "][" << level << "] " << message << "\n";

    /* Close the log file. */
    logFile.close();
}

/**
 * Logging errors into the application log file.
 */
void logError(std::string message)
{
    log(message, "ERROR");
}

/**
 * Logging info into the application log file.
 */
void logInfo(std::string message)
{
    log(message, "INFO");
}