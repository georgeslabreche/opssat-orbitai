#include <cstdlib>      /* For exit() */
#include <iostream>     /* For cout */
#include <sys/types.h>  /* For mkdkir */
#include <sys/stat.h>   /* For mkdkir */
#include <unistd.h>
#include <string>
#include <map>
#include <vector>

#include "Constants.hpp"
#include "Utils.hpp"
#include "PropertiesParser.hpp"
#include "HyperParameters.hpp"
#include "SocketServer.hpp"
#include "MochiMochiProxy.hpp"

using namespace std;

/* Modes */
enum class Mode {
    trainNew = 0,
    trainContinue = 1,
    infer = 2
};

/* Flag to keep track if models were loaded or not. */
int gModelsLoadedFlag = 0;

/**
 * Process the received command.
 * Returns flag indicating if the program loop should be exited or not.
 */
int processReceivedCommand(int mode, int dim, string *pReceivedCommand, MochiMochiProxy *pMochiMochiProxy);


/**
 * Main application function.
 */ 
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        logError(ERROR_INVALID_ARGS, "invalid number or arguments given, expected a path to the properties file.");
        exit(ERROR_INVALID_ARGS);
    }

    try
    {
        /* Create models and logs directory if they do not exist. */
        mkdir(DIR_PATH_LOGS, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        mkdir(DIR_PATH_MODELS, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        /* Check if given property file exists or not. */
        string propFilePath(argv[1]);
        if(!exists(propFilePath))
        {
            logError(ERROR_PROP_FILE_NOT_EXIST, "Properties file does not exist " + propFilePath);
            exit(ERROR_PROP_FILE_NOT_EXIST);
        }

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

        /* Instanciate the Proxy to the MochiMochi library Init the online ML algorithms */
        /* These online ML algorithms have been selectively enabled in the properties file. */
        MochiMochiProxy mochiMochiProxy(&propParser);

        /* Init the enabled algorithms. */
        mochiMochiProxy.initAlgorithms(dim, &hpMap);

        /* The buffer for received commands. */
        char buffer[COMMAND_BUFFER_LENGTH];

        /* Create Socket Server object. */
        SocketServer socketServer;

        /* Init the Socket Server and start listening for connections. */ 
        int connection;
        int sockfd;
        int errorCode = socketServer.initSocketServer(portNumber, &sockfd, &connection);

        if(errorCode != NO_ERROR)
        {
            logError(errorCode, "Failed to initialize Socket Server.");
            exit(errorCode);
        }

        /* Wait for a connection. */

        /* Read from the connection. */
        while(1)
        {
            /* Read received command. */
            auto bytesRead = read(connection, buffer, COMMAND_BUFFER_LENGTH);

            /* Get string representation of the command. */
            string receivedCmd(buffer, bytesRead);

            /* Print out received command. */
            //std::cout << "Received: " << receivedCmd << std::endl;

            /* Process the received command and break out the server loop if it's an exit command. */
            if(processReceivedCommand(mode, dim, &receivedCmd, &mochiMochiProxy) == 1)
            {
                // FIXME: break out of the loop instead of exit.
                exit(ERROR_PROCESSING_RECEIVED_COMMAND);
            }
        }

        /* Close connection and socket. */
        socketServer.shutdownSocketServer(&sockfd, &connection);

        /* Exit program. */
        exit(NO_ERROR);
    }
    catch(const exception& e)
    {
        // Log error.
        ostringstream oss;
        oss << "Error parsing arguments: " << e.what();
        logError(oss.str());
        
        // Exit program with error code.
        exit(ERROR_UNKNOWN);
    }
}


/**
 * Process the received command.
 * Returns flag indicating if the program loop should be exited or not.
 */
int processReceivedCommand(int mode, int dim, string *pReceivedCommand, MochiMochiProxy *pMochiMochiProxy)
{
    if(pReceivedCommand->compare(0, COMMAND_RESET_LENGTH, COMMAND_RESET) == 0)
    {
        /* Delete all model and log files. */
        pMochiMochiProxy->reset();
    }
    else if(pReceivedCommand->compare(0, COMMAND_SAVE_LENGTH, COMMAND_SAVE) == 0)
    {
        /* Save all models for the enabled algorithms. */
        pMochiMochiProxy->save(DIR_PATH_MODELS);
    }
    else if(pReceivedCommand->compare(0, COMMAND_EXIT_LENGTH, COMMAND_EXIT) == 0)
    {
        /* Exit the server loop. */
        return EXIT_PROGRAM_LOOP_YES;
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
                pMochiMochiProxy->trainAndSave(&input, dim, DIR_PATH_MODELS);
                break;

            case static_cast<int>(Mode::trainContinue):

                /* Load models if they weren't loaded already. */
                if(gModelsLoadedFlag == 0)
                {
                    /* Load all the models. */
                    pMochiMochiProxy->load(DIR_PATH_MODELS);

                    /* Mark models as having been loaded. */
                    gModelsLoadedFlag = 1;
                }

                /* Train and save models. */
                pMochiMochiProxy->trainAndSave(&input, dim, DIR_PATH_MODELS);

                break;

            case static_cast<int>(Mode::infer):

                /* Load models if they weren't loaded already. */
                if(gModelsLoadedFlag == 0)
                {
                    /* Load all the models. */
                    pMochiMochiProxy->load(DIR_PATH_MODELS);

                    /* Mark models as having been loaded. */
                    gModelsLoadedFlag = 1;
                }

                // TODO: Log prediction results.
                pMochiMochiProxy->infer(&input, dim);

                break;
        }
    }

    return EXIT_PROGRAM_LOOP_NO;
}