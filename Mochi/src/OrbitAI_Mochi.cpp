#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit()
#include <iostream> // For cout
#include <unistd.h> // For read

#include <sys/types.h> // For mkdkir
#include <sys/stat.h> // For mkdkir

#include <chrono> // For execution time calculations

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <ctime>

// For machine learning.
#include <mochimochi/binary_classifier.hpp>
#include <mochimochi/utility.hpp>


#define COMMAND_BUFFER_LENGTH                        100
#define SAVE_AFTER_UPDATE                              1    // Save/Serialize the models after each update.
#define LOG_TIMES                                      0    // Include timestamps and execution times in the CSV log files.

#define LOG_FILEPATH_TRAINING        "logs/training.csv"
#define LOG_FILEPATH_INFERENCE      "logs/inference.csv"
#define LOG_FILEPATH_ERROR              "logs/error.log"

// In case no port number is given.
#define DEFAULT_PORT_NUMBER                         9999

// Error codes.
#define ERROR_SUCCESS                                  0
#define ERROR_UNKNOWN                                  1
#define ERROR_PARSE_ARGS                               2
#define ERROR_INVALID_PORT_NUM                         3
#define ERROR_CREATE_SOCKET                            4
#define ERROR_BIND_PORT                                5
#define ERROR_LISTEN_SOCKET                            6
#define ERROR_GRAB_CONNECTION                          7

/**
 * Get a precise timestamp as a string.
 */
std::string getTimestamp();

/**
 * Logging errors into the error log file.
 */
void logError(std::string message);


/**
 * The main application loop.
 */
int main(int argc, char *argv[]) 
{
    // Main try/catch loop.
    try
    {
        // Assign port number to default number.
        int portNumber = DEFAULT_PORT_NUMBER;

        // Parse arguments.
        try
        {
            // Change port number to value given as command argument.
            if(argc == 2)
            {
                portNumber = std::stoi(argv[1]);

                if(portNumber <= 0)
                {
                    // Log error.
                    logError("Invalid port number: " + std::to_string(portNumber));

                    // Exit program with error code.
                    exit(ERROR_INVALID_PORT_NUM);
                }
            }
        }
        catch(const std::exception& e)
        {
            // Log error.
            std::ostringstream oss;
            oss << "Error parsing arguments: " << e.what();
            logError(oss.str());
            
            // Exit program with error code.
            exit(ERROR_PARSE_ARGS);
        } 

        // Create models and logs directory if they do not exist.
        mkdir("models", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        mkdir("logs", S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

        // Train command.
        const char* resetCmd = "reset";
        const int resetCmdLen = 5;

        // Train command.
        const char* trainCmd = "train";
        const int trainCmdLen = 5;

        // Infer command.
        const char* inferCmd = "infer";
        const int inferCmdLen = 5;

        // Save model command.
        const char* saveCmd = "save";
        const int saveCmdLen = 4;

        // Load model command.
        const char* loadCmd = "load";
        const int loadCmdLen = 4;

        // Exit command.
        const char* exitCmd = "exit";
        const int exitCmdLen = 4;

        // The hyperparameters
        const double hp_arow_r = 0.8;

        const double hp_scw_c = 1.0;
        const double hp_scw_eta = 0.95;

        const double hp_nherd_c = 0.1;
        const int hp_nherd_diagonal = 0;

        const double hp_pa_c = 0.1;
        const int hp_pa_select = 1;

        // ADAM
        ADAM adam_1D(1);
        ADAM adam_2D(2);
        ADAM adam_3D(3);
        ADAM adam_5D(5);

        // ARROW
        AROW arow_1D(1, hp_arow_r);
        AROW arow_2D(2, hp_arow_r);
        AROW arow_3D(3, hp_arow_r);
        AROW arow_5D(5, hp_arow_r);

        // SCW
        SCW scw_1D(1, hp_scw_c, hp_scw_eta);
        SCW scw_2D(2, hp_scw_c, hp_scw_eta);
        SCW scw_3D(3, hp_scw_c, hp_scw_eta);
        SCW scw_5D(5, hp_scw_c, hp_scw_eta);

        // NHERD
        NHERD nherd_1D(1, hp_nherd_c, hp_nherd_diagonal);
        NHERD nherd_2D(2, hp_nherd_c, hp_nherd_diagonal);
        NHERD nherd_3D(3, hp_nherd_c, hp_nherd_diagonal);
        NHERD nherd_5D(5, hp_nherd_c, hp_nherd_diagonal);

        // PA
        PA pa_1D(1, hp_pa_c, hp_pa_select);
        PA pa_2D(2, hp_pa_c, hp_pa_select);
        PA pa_3D(3, hp_pa_c, hp_pa_select);
        PA pa_5D(5, hp_pa_c, hp_pa_select);

        // The command buffer.
        char buffer[COMMAND_BUFFER_LENGTH];

        // Create a socket (IPv4, TCP)
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
            // Log error.
            logError("Failed to create socket. errno: " + std::to_string(errno));

            // Exit with error code.
            exit(ERROR_CREATE_SOCKET);
        }

        // Listen to port on any address
        sockaddr_in sockaddr;
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = INADDR_ANY;

        // Invoking htons is necessary to convert a number to network byte order.
        sockaddr.sin_port = htons(portNumber);

        // Bind to the given port.
        if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
        {
            // Log error.
            logError("Failed to bind to port " + std::to_string(portNumber) + ". errno: " + std::to_string(errno));

            // Exit with error code.
            exit(ERROR_BIND_PORT);
        }

        // Start listening. Hold at most 1 connection in the queue
        if (listen(sockfd, 1) < 0)
        {
            // Log error.
            logError("Failed to listen on socket. errno: " + std::to_string(errno));

            // Exit with error code.
            exit(ERROR_LISTEN_SOCKET);
        }

        // Grab a connection from the queue
        auto addrlen = sizeof(sockaddr);
        int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
        if (connection < 0)
        {
            // Log error.
            logError("Failed to grab connection. errno: " + std::to_string(errno));

            // Exit with error code.
            exit(ERROR_GRAB_CONNECTION);
        }

        // Start and end time points to measure operation execution times.
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point end;

        // Read from the connection
        while(1)
        {
            // Read received command.
            auto bytesRead = read(connection, buffer, COMMAND_BUFFER_LENGTH);

            // Get string representation of the command.
            std::string receivedCmd(buffer, bytesRead);

            // Print out received command.
            std::cout << "\nReceived: " << receivedCmd;

            if(receivedCmd.compare(0, resetCmdLen, resetCmd) == 0)
            {
                // ADAM
                remove("models/adam_1D");
                remove("models/adam_2D");
                remove("models/adam_3D");
                remove("models/adam_5D");

                // AROW
                remove("models/arow_1D");
                remove("models/arow_2D");
                remove("models/arow_3D");
                remove("models/arow_5D");

                // SCW
                remove("models/scw_1D");
                remove("models/scw_2D");
                remove("models/scw_3D");
                remove("models/scw_5D");

                // NHERD
                remove("models/nherd_1D");
                remove("models/nherd_2D");
                remove("models/nherd_3D");
                remove("models/nherd_5D");

                // PA
                remove("models/pa_1D");
                remove("models/pa_2D");
                remove("models/pa_3D");
                remove("models/pa_5D");

                // Delete log files.
                remove(LOG_FILEPATH_TRAINING);
                remove(LOG_FILEPATH_INFERENCE);
                remove(LOG_FILEPATH_ERROR);

                std::string response = "OK\n";
                send(connection, response.c_str(), response.size(), 0);

            }
            else if(receivedCmd.compare(0, loadCmdLen, loadCmd) == 0)  // Load saved models.
            {
                // Deserialize.
                
                // ADAM
                adam_1D.load("models/adam_1D");
                adam_2D.load("models/adam_2D");
                adam_3D.load("models/adam_3D");
                adam_5D.load("models/adam_5D");

                // AROW
                arow_1D.load("models/arow_1D");
                arow_2D.load("models/arow_2D");
                arow_3D.load("models/arow_3D");
                arow_5D.load("models/arow_5D");

                // SCW
                scw_1D.load("models/scw_1D");
                scw_2D.load("models/scw_2D");
                scw_3D.load("models/scw_3D");
                scw_5D.load("models/scw_5D");

                // NHERD
                nherd_1D.load("models/nherd_1D");
                nherd_2D.load("models/nherd_2D");
                nherd_3D.load("models/nherd_3D");
                nherd_5D.load("models/nherd_5D");

                // PA
                pa_1D.load("models/pa_1D");
                pa_2D.load("models/pa_2D");
                pa_3D.load("models/pa_3D");
                pa_5D.load("models/pa_5D");

                std::string response = "OK\n";
                send(connection, response.c_str(), response.size(), 0);
            }
            else if(receivedCmd.compare(0, saveCmdLen, saveCmd) == 0)// Save models.
            {
                // Serialize.

                // ADAM
                adam_1D.save("models/adam_1D");
                adam_2D.save("models/adam_2D");
                adam_3D.save("models/adam_3D");
                adam_5D.save("models/adam_5D");

                // AROW
                arow_1D.save("models/arow_1D");
                arow_2D.save("models/arow_2D");
                arow_3D.save("models/arow_3D");
                arow_5D.save("models/arow_5D");

                // SCW
                scw_1D.save("models/scw_1D");
                scw_2D.save("models/scw_2D");
                scw_3D.save("models/scw_3D");
                scw_5D.save("models/scw_5D");

                // NHERD
                nherd_1D.save("models/nherd_1D");
                nherd_2D.save("models/nherd_2D");
                nherd_3D.save("models/nherd_3D");
                nherd_5D.save("models/nherd_5D");

                // PA
                pa_1D.save("models/pa_1D");
                pa_2D.save("models/pa_2D");
                pa_3D.save("models/pa_3D");
                pa_5D.save("models/pa_5D");

                std::string response = "OK\n";
                send(connection, response.c_str(), response.size(), 0);
            }
            else if(receivedCmd.compare(0, exitCmdLen, exitCmd) == 0)
            {
                std::string response = "BYE\n";
                send(connection, response.c_str(), response.size(), 0);

                // Exit server loop.
                break;
            }
            else if(receivedCmd.compare(0, trainCmdLen, trainCmd) == 0)
            {
                /**
                 * Expected command string format:
                 *      train <label:int> <pd1:float> <pd2:float> <pd3:float> <pd4:float> <pd5:float> <pd6:float> <timestamp:long>
                 * 
                 * e.g.: 
                 *  train 1 0.11 0.22 0.33 0.44 0.55 1.23 1615253200322
                 *  train 0 0.11 0.22 0.33 0.44 0.55 0.32 1615253200322
                 **/
                try
                {
                    std::string cmdString(buffer, COMMAND_BUFFER_LENGTH);
                    std::istringstream cmdStringStream(cmdString);

                    std::vector<std::string> cmdTokens
                    {
                        std::istream_iterator<std::string>{cmdStringStream},
                        std::istream_iterator<std::string>{}
                    };

                    // Parse input string label int and PD float.
                    int label = std::stoi(cmdTokens[1]);
                    
                    // Photodiode values.
                    float pd1 = std::stof(cmdTokens[2]);
                    float pd2 = std::stof(cmdTokens[3]);
                    float pd3 = std::stof(cmdTokens[4]);
                    float pd4 = std::stof(cmdTokens[5]);
                    float pd5 = std::stof(cmdTokens[6]);
                    float pd6 = std::stof(cmdTokens[7]);

                    // PD acquisition timestamp.
                    long timestamp = std::stol(cmdTokens[8]);

                    // Training input error.
                    if(!(label == 0 || label == -1 || label == 1))
                    {
                        std::string response = "ERROR\n";
                        send(connection, response.c_str(), response.size(), 0);
                    }
                    else // Valid training input.
                    {
                        // Features inputs for PD6.
                        // PD6 is located on the same surface as the camera.

                        // 1D feature space.
                        std::ostringstream ss1;
                        ss1 << pd6;
                        std::string s1(ss1.str());

                        // 2D feature space.
                        std::ostringstream ss2;
                        ss2 << pd6 * pd6;
                        std::string s2(ss2.str());

                        // 3D feature space
                        std::ostringstream ss3;
                        ss3 << pd6 * pd6 * pd6;
                        std::string s3(ss3.str());

                        // Training data for 1D, 2D, and 3D feature spaces.
                        std::string line_1D;
                        std::string line_2D;
                        std::string line_3D;

                        // Training data for 5D feature space: all photodiode elevation angle values except PD6.
                        // This is to experiment with training a redundant model that can be used in case PD6 fails.
                        std::string line_5D;

                        if(label == 1)
                        {
                            line_1D = "+1 1:" + s1;
                            line_2D = "+1 1:" + s1 + " 2:" + s2;
                            line_3D = "+1 1:" + s1 + " 2:" + s2 + " 3:" + s3;
                            line_5D = "+1 1:" + std::to_string(pd1) + " 2:" + std::to_string(pd2) + " 3:" + std::to_string(pd3) + " 4:" + std::to_string(pd4) + " 5:" + std::to_string(pd5);
                        }
                        else if(label == 0 || label == -1)
                        {
                            line_1D = "-1 1:" + s1;
                            line_2D = "-1 1:" + s1 + " 2:" + s2;
                            line_3D = "-1 1:" + s1 + " 2:" + s2 + " 3:" + s3;
                            line_5D = "-1 1:" + std::to_string(pd1) + " 2:" + std::to_string(pd2) + " 3:" + std::to_string(pd3) + " 4:" + std::to_string(pd4) + " 5:" + std::to_string(pd5);
                        }

                        // Read data.
                        auto data_1D = utility::read_ones<int>(line_1D, 1);
                        auto data_2D = utility::read_ones<int>(line_2D, 2);
                        auto data_3D = utility::read_ones<int>(line_3D, 3);
                        auto data_5D = utility::read_ones<int>(line_5D, 5);

                        // Train the models.

                        /** 
                         * ADAM
                         */
                        
                        // 1D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        adam_1D.update(data_1D.second, data_1D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_adam_1D = end - start;

                        // 2D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        adam_2D.update(data_2D.second, data_2D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_adam_2D = end - start;

                        // 3D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        adam_3D.update(data_3D.second, data_3D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_adam_3D = end - start;

                        // 5D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        adam_5D.update(data_5D.second, data_5D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_adam_5D = end - start;

                        /** 
                         * AROW
                         */

                        // 1D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        arow_1D.update(data_1D.second, data_1D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_arow_1D = end - start;

                        // 2D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        arow_2D.update(data_2D.second, data_2D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_arow_2D = end - start;

                        // 3D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        arow_3D.update(data_3D.second, data_3D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_arow_3D = end - start;

                        // 5D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        arow_5D.update(data_5D.second, data_5D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_arow_5D = end - start;

                        /**
                         * SCW
                         */

                        // 1D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        scw_1D.update(data_1D.second, data_1D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_scw_1D = end - start;

                        // 2D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        scw_2D.update(data_2D.second, data_2D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_scw_2D = end - start;

                        // 3D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        scw_3D.update(data_3D.second, data_3D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_scw_3D = end - start;

                        // 5D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        scw_5D.update(data_5D.second, data_5D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_scw_5D = end - start;

                        /**
                         * NHERD
                         */

                        // 1D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        nherd_1D.update(data_1D.second, data_1D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_nherd_1D = end - start;

                        // 2D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        nherd_2D.update(data_2D.second, data_2D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_nherd_2D = end - start;

                        // 3D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        nherd_3D.update(data_3D.second, data_3D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_nherd_3D = end - start;

                        // 5D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        nherd_5D.update(data_5D.second, data_5D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_nherd_5D = end - start;

                        /**
                         * PA
                         */

                        // 1D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        pa_1D.update(data_1D.second, data_1D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_pa_1D = end - start;

                        // 2D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        pa_2D.update(data_2D.second, data_2D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_pa_2D = end - start;

                        // 3D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        pa_3D.update(data_3D.second, data_3D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_pa_3D = end - start;

                        // 5D feature space. Record execution time in milliseconds.
                        start = std::chrono::high_resolution_clock::now();
                        pa_5D.update(data_5D.second, data_5D.first);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> updateTime_pa_5D = end - start;

#if SAVE_AFTER_UPDATE
                        // Save the models if set to do so.

                        // ADAM
                        adam_1D.save("models/adam_1D");
                        adam_2D.save("models/adam_2D");
                        adam_3D.save("models/adam_3D");
                        adam_5D.save("models/adam_5D");

                        // AROW
                        arow_1D.save("models/arow_1D");
                        arow_2D.save("models/arow_2D");
                        arow_3D.save("models/arow_3D");
                        arow_5D.save("models/arow_5D");

                        // SCW
                        scw_1D.save("models/scw_1D");
                        scw_2D.save("models/scw_2D");
                        scw_3D.save("models/scw_3D");
                        scw_5D.save("models/scw_5D");

                        // NHERD
                        nherd_1D.save("models/nherd_1D");
                        nherd_2D.save("models/nherd_2D");
                        nherd_3D.save("models/nherd_3D");
                        nherd_5D.save("models/nherd_5D");

                        // PA
                        pa_1D.save("models/pa_1D");
                        pa_2D.save("models/pa_2D");
                        pa_3D.save("models/pa_3D");
                        pa_5D.save("models/pa_5D");
#endif
                        // Opening the training log file.
                        std::ofstream trainingLogFile;
                        trainingLogFile.open(LOG_FILEPATH_TRAINING, std::ios_base::out | std::ios_base::app);

                        // Creating the training row string to append to the log file.
                        std::string trainingLogFileRow = 
#if LOG_TIMES
                            std::to_string(timestamp) + "," + 
#endif 
                            std::to_string(pd1) + "," + std::to_string(pd2) + "," + std::to_string(pd3) + "," + std::to_string(pd4) + "," + std::to_string(pd5) + "," + std::to_string(pd6) + "," + std::to_string(label) 
#if LOG_TIMES
                            + ","
                            + std::to_string(updateTime_adam_1D.count()) + "," + std::to_string(updateTime_adam_2D.count()) + "," + std::to_string(updateTime_adam_3D.count()) + "," + std::to_string(updateTime_adam_5D.count()) + ","
                            + std::to_string(updateTime_arow_1D.count()) + "," + std::to_string(updateTime_arow_2D.count()) + "," + std::to_string(updateTime_arow_3D.count()) + "," + std::to_string(updateTime_arow_5D.count()) + ","
                            + std::to_string(updateTime_scw_1D.count()) + "," + std::to_string(updateTime_scw_2D.count()) + "," + std::to_string(updateTime_scw_3D.count()) + "," + std::to_string(updateTime_scw_5D.count()) + ","
                            + std::to_string(updateTime_nherd_1D.count()) + "," + std::to_string(updateTime_nherd_2D.count()) + "," + std::to_string(updateTime_nherd_3D.count()) + "," + std::to_string(updateTime_nherd_5D.count()) + ","
                            + std::to_string(updateTime_pa_1D.count()) + "," + std::to_string(updateTime_pa_2D.count()) + "," + std::to_string(updateTime_pa_3D.count())+ "," + std::to_string(updateTime_pa_5D.count())
#endif                         
                            + "\n";

                        // Append the training row data into the log file.
                        trainingLogFile << trainingLogFileRow;

                        // Close the log files.
                        trainingLogFile.close();

                        std::string response = "OK\n";
                        send(connection, response.c_str(), response.size(), 0);
                    }
                }
                catch (const std::exception& e)
                {
                    // Log error.
                    std::ostringstream oss;
                    oss << "Unhandled training error: " << e.what();
                    logError(oss.str());

                    // Error response
                    std::string response = "ERROR\n";
                    send(connection, response.c_str(), response.size(), 0);
                }
            }
            else if(receivedCmd.compare(0, inferCmdLen, inferCmd) == 0)
            {
                /**
                 * Expected command string format:
                 *      infer <expected_label:int> <pd1:float> <pd2:float> <pd3:float> <pd4:float> <pd5:float> <pd6:float> <timestamp:long>
                 * 
                 * e.g.: 
                 *  infer +1 0.11 0.22 0.33 0.44 0.55 1.23 1615253200322
                 *  infer -1 0.32 0.22 0.33 0.44 0.55 1615253200322
                 **/

                try
                {
                    std::string cmdString(buffer, COMMAND_BUFFER_LENGTH);
                    std::istringstream cmdStringStream(cmdString);

                    std::vector<std::string> cmdTokens
                    {
                        std::istream_iterator<std::string>{cmdStringStream}, 
                        std::istream_iterator<std::string>{}
                    };

                    // Parse input string for expected label int and PD float.
                    int label = std::stoi(cmdTokens[1]);

                    // Photodiode values.
                    float pd1 = std::stof(cmdTokens[2]);
                    float pd2 = std::stof(cmdTokens[3]);
                    float pd3 = std::stof(cmdTokens[4]);
                    float pd4 = std::stof(cmdTokens[5]);
                    float pd5 = std::stof(cmdTokens[6]);
                    float pd6 = std::stof(cmdTokens[7]);

                    // PD acquisition timestamp.
                    long timestamp = std::stol(cmdTokens[8]);

                    // inference input error.
                    if(!(label == 0 || label == -1 || label == 1))
                    {
                        std::string response = "ERROR\n";
                        send(connection, response.c_str(), response.size(), 0);
                    }
                    else // Valid inference input.
                    {
                        // Features.
                        std::ostringstream ss1;
                        ss1 << pd6;
                        std::string s1(ss1.str());

                        std::ostringstream ss2;
                        ss2 << pd6 * pd6;
                        std::string s2(ss2.str());

                        std::ostringstream ss3;
                        ss3 << pd6 * pd6 * pd6;
                        std::string s3(ss3.str());

                        // Inference data for 1D, 2D, and 3D feature spaces.
                        std::string line_1D;
                        std::string line_2D;
                        std::string line_3D;

                        // Training data for 5D feature space: all photodiode elevation angle values except PD6.
                        // This is to experiment with training a redundant model that can be used in case PD6 fails.
                        std::string line_5D;

                        if(label == 1)
                        {
                            line_1D = "+1 1:" + s1;
                            line_2D = "+1 1:" + s1 + " 2:" + s2;
                            line_3D = "+1 1:" + s1 + " 2:" + s2 + " 3:" + s3;
                            line_5D = "+1 1:" + std::to_string(pd1) + " 2:" + std::to_string(pd2) + " 3:" + std::to_string(pd3) + " 4:" + std::to_string(pd4) + " 5:" + std::to_string(pd5);
                        }
                        else if(label == 0 || label == -1)
                        {

                            line_1D = "-1 1:" + s1;
                            line_2D = "-1 1:" + s1 + " 2:" + s2;
                            line_3D = "-1 1:" + s1 + " 2:" + s2 + " 3:" + s3;
                            line_5D = "-1 1:" + std::to_string(pd1) + " 2:" + std::to_string(pd2) + " 3:" + std::to_string(pd3) + " 4:" + std::to_string(pd4) + " 5:" + std::to_string(pd5);
                        }

                        // Read data.
                        auto data_1D = utility::read_ones<int>(line_1D, 1);
                        auto data_2D = utility::read_ones<int>(line_2D, 2);
                        auto data_3D = utility::read_ones<int>(line_3D, 3);
                        auto data_5D = utility::read_ones<int>(line_5D, 5);

                        /**
                         * ADAM.
                         */
                        start = std::chrono::high_resolution_clock::now();
                        int adam_pred_1D = adam_1D.predict(data_1D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_adam_1D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int adam_pred_2D = adam_2D.predict(data_2D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_adam_2D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int adam_pred_3D = adam_3D.predict(data_3D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_adam_3D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int adam_pred_5D = adam_5D.predict(data_5D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_adam_5D = end - start;

                        /**
                         * AROW.
                         */
                        start = std::chrono::high_resolution_clock::now();
                        int arow_pred_1D = arow_1D.predict(data_1D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_arow_1D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int arow_pred_2D = arow_2D.predict(data_2D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_arow_2D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int arow_pred_3D = arow_3D.predict(data_3D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_arow_3D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int arow_pred_5D = arow_5D.predict(data_5D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_arow_5D = end - start;

                        /**
                         * SCW.
                         */
                        start = std::chrono::high_resolution_clock::now();
                        int scw_pred_1D = scw_1D.predict(data_1D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_scw_1D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int scw_pred_2D = scw_2D.predict(data_2D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_scw_2D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int scw_pred_3D = scw_3D.predict(data_3D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_scw_3D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int scw_pred_5D = scw_5D.predict(data_5D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_scw_5D = end - start;

                        /**
                         * NHERD.
                         */
                        start = std::chrono::high_resolution_clock::now();
                        int nherd_pred_1D = nherd_1D.predict(data_1D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_nherd_1D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int nherd_pred_2D = nherd_2D.predict(data_2D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_nherd_2D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int nherd_pred_3D = nherd_3D.predict(data_3D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_nherd_3D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int nherd_pred_5D = nherd_5D.predict(data_5D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_nherd_5D = end - start;

                        /**
                         * PA.
                         */
                        start = std::chrono::high_resolution_clock::now();
                        int pa_pred_1D = pa_1D.predict(data_1D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_pa_1D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int pa_pred_2D = pa_2D.predict(data_2D.second);
                        end = std::chrono::high_resolution_clock::now();
                        std::chrono::duration<double, std::milli> predictTime_pa_2D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int pa_pred_3D = pa_3D.predict(data_3D.second);
                        end = std::chrono::high_resolution_clock::now(); 
                        std::chrono::duration<double, std::milli> predictTime_pa_3D = end - start;

                        start = std::chrono::high_resolution_clock::now();
                        int pa_pred_5D = pa_5D.predict(data_5D.second);
                        end = std::chrono::high_resolution_clock::now(); 
                        std::chrono::duration<double, std::milli> predictTime_pa_5D = end - start;

                        // The inference log file.
                        std::ofstream inferenceLogFile;
                        inferenceLogFile.open(LOG_FILEPATH_INFERENCE, std::ios_base::out | std::ios_base::app);

                        // Concatenate the prediction results.
                        std::string predictions = std::to_string(adam_pred_1D) + "," + std::to_string(adam_pred_2D) + "," + std::to_string(adam_pred_3D) + "," + std::to_string(adam_pred_5D) + ","
                            + std::to_string(arow_pred_1D) + "," + std::to_string(arow_pred_2D) + "," + std::to_string(arow_pred_3D) + "," + std::to_string(arow_pred_5D) + ","
                            + std::to_string(scw_pred_1D) + "," + std::to_string(scw_pred_2D) + "," + std::to_string(scw_pred_3D) + "," + std::to_string(scw_pred_5D) + ","
                            + std::to_string(nherd_pred_1D) + "," + std::to_string(nherd_pred_2D) + "," + std::to_string(nherd_pred_3D) + "," + std::to_string(nherd_pred_5D) + ","
                            + std::to_string(pa_pred_1D) + "," + std::to_string(pa_pred_2D) + "," + std::to_string(pa_pred_3D) + "," + std::to_string(pa_pred_5D);

                        // Creating the inference results row string to append to the log file.
                        std::string inferenceLogFileRow =
    #if LOG_TIMES
                            std::to_string(timestamp) + "," + 
    #endif                     
                            std::to_string(pd1) + "," + std::to_string(pd2) + "," + std::to_string(pd3) + "," + std::to_string(pd4) + "," + std::to_string(pd5) + "," + std::to_string(pd6) + "," + std::to_string(label) + ","
                            + predictions
    #if LOG_TIMES
                            + ","
                            + std::to_string(predictTime_adam_1D.count()) + "," + std::to_string(predictTime_adam_2D.count()) + "," + std::to_string(predictTime_adam_3D.count()) + "," + std::to_string(predictTime_adam_5D.count()) + ","
                            + std::to_string(predictTime_arow_1D.count()) + "," + std::to_string(predictTime_arow_2D.count()) + "," + std::to_string(predictTime_arow_3D.count()) + "," + std::to_string(predictTime_arow_5D.count()) + ","
                            + std::to_string(predictTime_scw_1D.count()) + "," + std::to_string(predictTime_scw_2D.count()) + "," + std::to_string(predictTime_scw_3D.count()) + "," + std::to_string(predictTime_scw_5D.count()) + ","
                            + std::to_string(predictTime_nherd_1D.count()) + "," + std::to_string(predictTime_nherd_2D.count()) + "," + std::to_string(predictTime_nherd_3D.count()) + "," + std::to_string(predictTime_nherd_5D.count()) + ","
                            + std::to_string(predictTime_pa_1D.count()) + "," + std::to_string(predictTime_pa_2D.count()) + "," + std::to_string(predictTime_pa_3D.count()) + "," + std::to_string(predictTime_pa_5D.count())
    #endif
                            + "\n";

                        // Append infer result row to CSV log file.
                        inferenceLogFile << inferenceLogFileRow;

                        // Close file.
                        inferenceLogFile.close();

                        // Send row to client.
                        send(connection, (predictions + "\n").c_str(), (predictions + "\n").size(), 0);
                    }
                }
                catch(const std::exception& e)
                {
                    // Log error.
                    std::ostringstream oss;
                    oss << "Unhandled inference error: " << e.what();
                    logError(oss.str());

                    // Send response.
                    std::string response = "ERROR\n";
                    send(connection, response.c_str(), response.size(), 0);
                }
            }
            else
            {
                // Log error.
                logError("Received invalid command: " + receivedCmd);

                std::string response = "INVALID\n";
                send(connection, response.c_str(), response.size(), 0);
            }
        }

        // Close the connection
        if(connection)
        {
            close(connection);
        }

        // Close the socket.
        if(sockfd)
        {
            close(sockfd);
        }

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
}


/**
 * Get a precise timestamp as a string.
 * Taken from: https://gist.github.com/bschlinker/844a88c09dcf7a61f6a8df1e52af7730
 */
std::string getTimestamp() 
{
  // 
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
 * Logging errors into the error log file.
 */
void logError(std::string message)
{
    // Print out error message.
    std::cout << message << std::endl;
    
    // Open the error log file.
    std::ofstream errorLogFile;
    errorLogFile.open(LOG_FILEPATH_ERROR, std::ios_base::out | std::ios_base::app);

    // Append the log message into the error log file.
    errorLogFile << "[" << getTimestamp() << "] " << message << "\n";

    // Close the log file.
    errorLogFile.close();
}