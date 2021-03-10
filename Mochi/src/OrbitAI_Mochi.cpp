#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read

#include <sys/types.h> // For mkdkir
#include <sys/stat.h> // For mkdkir

#include <chrono> // For execution time calculations

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

// For machine learning.
#include <mochimochi/binary_classifier.hpp>
#include <mochimochi/utility.hpp>


#define COMMAND_BUFFER_LENGTH                        100
#define SAVE_AFTER_UPDATE                              1
#define LOG_TIMES                                      1

#define LOG_FILEPATH_TRAINING        "logs/training.csv"
#define LOG_FILEPATH_INFERENCE      "logs/inference.csv"
#define LOG_FILEPATH_ERROR              "logs/error.log"

// In case no port number is given.
#define DEFAULT_PORT_NUMBER                         9999 

int main(int argc, char *argv[]) 
{
    // Main try/catch loop.
    try
    {
        // Assign port number to default number.
        int portNumber = DEFAULT_PORT_NUMBER;

        // Change port number to value given as command argument.
        if(argc == 2)
        {
            portNumber = std::stoi(argv[1]);
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
        //ADAM adam_1D(1);
        //ADAM adam_2D(2);
        //ADAM adam_3D(3);

        // ARROW
        AROW arow_1D(1, hp_arow_r);
        AROW arow_2D(2, hp_arow_r);
        AROW arow_3D(3, hp_arow_r);

        // SCW
        SCW scw_1D(1, hp_scw_c, hp_scw_eta);
        SCW scw_2D(2, hp_scw_c, hp_scw_eta);
        SCW scw_3D(3, hp_scw_c, hp_scw_eta);

        // NHERD
        NHERD nherd_1D(1, hp_nherd_c, hp_nherd_diagonal);
        NHERD nherd_2D(2, hp_nherd_c, hp_nherd_diagonal);
        NHERD nherd_3D(3, hp_nherd_c, hp_nherd_diagonal);

        // PA
        PA pa_1D(1, hp_pa_c, hp_pa_select);
        PA pa_2D(2, hp_pa_c, hp_pa_select);
        PA pa_3D(3, hp_pa_c, hp_pa_select);

        // The command buffer.
        char buffer[COMMAND_BUFFER_LENGTH];

        // Create a socket (IPv4, TCP)
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1)
        {
            std::cout << "Failed to create socket. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
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
            std::cout << "Failed to bind to port " << portNumber << ". errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }

        // Start listening. Hold at most 1 connection in the queue
        if (listen(sockfd, 1) < 0)
        {
            std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
        }

        // Grab a connection from the queue
        auto addrlen = sizeof(sockaddr);
        int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
        if (connection < 0)
        {
            std::cout << "Failed to grab connection. errno: " << errno << std::endl;
            exit(EXIT_FAILURE);
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
                //remove("models/adam_1D");
                //remove("models/adam_2D");
                //remove("models/adam_3D");

                // AROW
                remove("models/arow_1D");
                remove("models/arow_2D");
                remove("models/arow_3D");

                // SCW
                remove("models/scw_1D");
                remove("models/scw_2D");
                remove("models/scw_3D");

                // NHERD
                remove("models/nherd_1D");
                remove("models/nherd_2D");
                remove("models/nherd_3D");

                // PA
                remove("models/pa_1D");
                remove("models/pa_2D");
                remove("models/pa_3D");

                // Logs
                remove(LOG_FILEPATH_TRAINING);
                remove(LOG_FILEPATH_INFERENCE);

                std::string response = "OK\n";
                send(connection, response.c_str(), response.size(), 0);

            }
            else if(receivedCmd.compare(0, loadCmdLen, loadCmd) == 0)  // Load saved models.
            {
                // Deserialize.
                
                // ADAM
                //adam_1D.load("models/adam_1D");
                //adam_2D.load("models/adam_2D");
                //adam_3D.load("models/adam_3D");

                // AROW
                arow_1D.load("models/arow_1D");
                arow_2D.load("models/arow_2D");
                arow_3D.load("models/arow_3D");

                // SCW
                scw_1D.load("models/scw_1D");
                scw_2D.load("models/scw_2D");
                scw_3D.load("models/scw_3D");

                // NHERD
                nherd_1D.load("models/nherd_1D");
                nherd_2D.load("models/nherd_2D");
                nherd_3D.load("models/nherd_3D");

                // PA
                pa_1D.load("models/pa_1D");
                pa_2D.load("models/pa_2D");
                pa_3D.load("models/pa_3D");

                std::string response = "OK\n";
                send(connection, response.c_str(), response.size(), 0);
            }
            else if(receivedCmd.compare(0, saveCmdLen, saveCmd) == 0)// Save models.
            {
                // Serialize.

                // ADAM
                //adam_1D.save("models/adam_1D");
                //adam_2D.save("models/adam_2D");
                //adam_3D.save("models/adam_3D");

                // AROW
                arow_1D.save("models/arow_1D");
                arow_2D.save("models/arow_2D");
                arow_3D.save("models/arow_3D");

                // SCW
                scw_1D.save("models/scw_1D");
                scw_2D.save("models/scw_2D");
                scw_3D.save("models/scw_3D");

                // NHERD
                nherd_1D.save("models/nherd_1D");
                nherd_2D.save("models/nherd_2D");
                nherd_3D.save("models/nherd_3D");

                // PA
                pa_1D.save("models/pa_1D");
                pa_2D.save("models/pa_2D");
                pa_3D.save("models/pa_3D");

                std::string response = "OK\n";
                send(connection, response.c_str(), response.size(), 0);
            }
            else if(receivedCmd.compare(0, exitCmdLen, exitCmd) == 0) // Exit server loop.
            {
                std::string response = "BYE\n";
                send(connection, response.c_str(), response.size(), 0);
                break;
            }
            else if(receivedCmd.compare(0, trainCmdLen, trainCmd) == 0)
            {
                /**
                 * Expected command string format:
                 *      train <label:int> <pd:float> <timestamp:long>
                 * 
                 * e.g.: 
                 *  train 1 1.23 1615253200322
                 *  train 0 0.32 1615253200322
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
                    float pd = std::stof(cmdTokens[2]);
                    long timestamp = std::stol(cmdTokens[3]);

                    // Training input error.
                    if(!(label == 0 || label == -1 || label == 1))
                    {
                        std::string response = "ERROR\n";
                        send(connection, response.c_str(), response.size(), 0);
                    }
                    else // Valid training input.
                    {
                        // Features.
                        std::ostringstream ss1;
                        ss1 << pd;
                        std::string s1(ss1.str());

                        std::ostringstream ss2;
                        ss2 << pd * pd;
                        std::string s2(ss2.str());

                        std::ostringstream ss3;
                        ss3 << pd * pd * pd;
                        std::string s3(ss3.str());

                        // Training data for 1D, 2D, and 3D feature spaces.
                        std::string line_1D;
                        std::string line_2D;
                        std::string line_3D;

                        if(label == 1)
                        {
                            line_1D = "+1 1:" + s1;
                            line_2D = "+1 1:" + s1 + " 2:" + s2;
                            line_3D = "+1 1:" + s1 + " 2:" + s2 + " 3:" + s3;
                        }
                        else if(label == 0 || label == -1)
                        {
                            line_1D = "-1 1:" + s1;
                            line_2D = "-1 1:" + s1 + " 2:" + s2;
                            line_3D = "-1 1:" + s1 + " 2:" + s2 + " 3:" + s3;
                        }

                        // Read data.
                        auto data_1D = utility::read_ones<int>(line_1D, 1);
                        auto data_2D = utility::read_ones<int>(line_2D, 2);
                        auto data_3D = utility::read_ones<int>(line_3D, 3);

                        // Train the models.

                        // ADAM
                        //adam_2D.update(data_1D.second, data_1D.first);
                        //adam_2D.update(data_2D.second, data_2D.first);
                        //adam_3D.update(data_3D.second, data_3D.first);

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


    #if SAVE_AFTER_UPDATE
                        // Save the models if set to do so.

                        // AROW
                        arow_1D.save("models/arow_1D");
                        arow_2D.save("models/arow_2D");
                        arow_3D.save("models/arow_3D");

                        // SCW
                        scw_1D.save("models/scw_1D");
                        scw_2D.save("models/scw_2D");
                        scw_3D.save("models/scw_3D");

                        // NHERD
                        nherd_1D.save("models/nherd_1D");
                        nherd_2D.save("models/nherd_2D");
                        nherd_3D.save("models/nherd_3D");

                        // PA
                        pa_1D.save("models/pa_1D");
                        pa_2D.save("models/pa_2D");
                        pa_3D.save("models/pa_3D");
    #endif

                        // Opening the training log file.
                        std::ofstream trainingLogFile;
                        trainingLogFile.open(LOG_FILEPATH_TRAINING, std::ios_base::out | std::ios_base::app);

                        // Creating the training row string to append to the log file.
                        std::string trainingLogFileRow = 
    #if LOG_TIMES
                            std::to_string(timestamp) + "," + 
    #endif 
                            std::to_string(pd) + "," + std::to_string(label) 
    #if LOG_TIMES
                            + ","
                            + std::to_string(updateTime_arow_1D.count()) + "," + std::to_string(updateTime_arow_2D.count()) + "," + std::to_string(updateTime_arow_3D.count()) + ","
                            + std::to_string(updateTime_scw_1D.count()) + "," + std::to_string(updateTime_scw_2D.count()) + "," + std::to_string(updateTime_scw_3D.count()) + ","
                            + std::to_string(updateTime_nherd_1D.count()) + "," + std::to_string(updateTime_nherd_2D.count()) + "," + std::to_string(updateTime_nherd_3D.count()) + ","
                            + std::to_string(updateTime_pa_1D.count()) + "," + std::to_string(updateTime_pa_2D.count()) + "," + std::to_string(updateTime_pa_3D.count())
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
                    // TODO: Log error in log file.
                    std::string response = "ERROR\n";
                    send(connection, response.c_str(), response.size(), 0);
                }
            }
            else if(receivedCmd.compare(0, inferCmdLen, inferCmd) == 0)
            {
                /**
                 * Expected command string format:
                 *      infer <expected_label:int> <pd:float> <timestamp:long>
                 * 
                 * e.g.: 
                 *  infer +1 1.23 1615253200322
                 *  infer -1 0.32 1615253200322
                 **/

                try
                {
                    std::string cmdString(buffer, COMMAND_BUFFER_LENGTH);
                    std::istringstream cmdStringStream(cmdString);

                    std::vector<std::string> cmdTokens{
                        std::istream_iterator<std::string>{cmdStringStream}, 
                        std::istream_iterator<std::string>{}
                    };

                    // Parse input string for expected label int and PD float.
                    int label = std::stoi(cmdTokens[1]);
                    float pd = std::stof(cmdTokens[2]);
                    long timestamp = std::stol(cmdTokens[3]);

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
                        ss1 << pd;
                        std::string s1(ss1.str());

                        std::ostringstream ss2;
                        ss2 << pd * pd;
                        std::string s2(ss2.str());

                        std::ostringstream ss3;
                        ss3 << pd * pd * pd;
                        std::string s3(ss3.str());

                        // Inference data for 1D, 2D, and 3D feature spaces.
                        std::string line_1D;
                        std::string line_2D;
                        std::string line_3D;

                        if(label == 1)
                        {
                            line_1D = "+1 1:" + s1;
                            line_2D = "+1 1:" + s1 + " 2:" + s2;
                            line_3D = "+1 1:" + s1 + " 2:" + s2 + " 3:" + s3;
                        }
                        else if(label == 0 || label == -1)
                        {

                            line_1D = "-1 1:" + s1;
                            line_2D = "-1 1:" + s1 + " 2:" + s2;
                            line_3D = "-1 1:" + s1 + " 2:" + s2 + " 3:" + s3;
                        }

                        // Read data.
                        auto data_1D = utility::read_ones<int>(line_1D, 1);
                        auto data_2D = utility::read_ones<int>(line_2D, 2);
                        auto data_3D = utility::read_ones<int>(line_3D, 3);

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

                        // The inference log file.
                        std::ofstream inferenceLogFile;
                        inferenceLogFile.open(LOG_FILEPATH_INFERENCE, std::ios_base::out | std::ios_base::app);

                        // Creating the inference results row string to append to the log file.
                        std::string inferenceLogFileRow =
    #if LOG_TIMES
                            std::to_string(timestamp) + "," + 
    #endif                     
                            std::to_string(pd) + "," + std::to_string(label) + ","
                            + std::to_string(arow_pred_1D) + "," + std::to_string(arow_pred_2D) + "," + std::to_string(arow_pred_3D) + ","
                            + std::to_string(scw_pred_1D) + "," + std::to_string(scw_pred_2D) + "," + std::to_string(scw_pred_3D) + ","
                            + std::to_string(nherd_pred_1D) + "," + std::to_string(nherd_pred_2D) + "," + std::to_string(nherd_pred_3D) + ","
                            + std::to_string(pa_pred_1D) + "," + std::to_string(pa_pred_2D) + "," + std::to_string(pa_pred_3D)
    #if LOG_TIMES
                            + ","
                            + std::to_string(predictTime_arow_1D.count()) + "," + std::to_string(predictTime_arow_2D.count()) + "," + std::to_string(predictTime_arow_3D.count()) + ","
                            + std::to_string(predictTime_scw_1D.count()) + "," + std::to_string(predictTime_scw_2D.count()) + "," + std::to_string(predictTime_scw_3D.count()) + ","
                            + std::to_string(predictTime_nherd_1D.count()) + "," + std::to_string(predictTime_nherd_2D.count()) + "," + std::to_string(predictTime_nherd_3D.count()) + ","
                            + std::to_string(predictTime_pa_1D.count()) + "," + std::to_string(predictTime_pa_2D.count()) + "," + std::to_string(predictTime_pa_3D.count())
    #endif
                            + "\n";

                        // Append infer result row to CSV log file.
                        inferenceLogFile << inferenceLogFileRow;

                        // Close file.
                        inferenceLogFile.close();

                        // Send row to client.
                        send(connection, inferenceLogFileRow.c_str(), inferenceLogFileRow.size(), 0);
                    }
                }
                catch(const std::exception& e)
                {
                    // TODO: Log error in log file.
                    std::string response = "ERROR\n";
                    send(connection, response.c_str(), response.size(), 0);

                    std::cerr << e.what() << '\n';
                }
            }
            else
            {
                std::cout << "\nReceived invalid command: " << receivedCmd;
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
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
    }
}
