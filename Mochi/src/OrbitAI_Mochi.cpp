#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

// For machine learning.
#include <mochimochi/binary_classifier.hpp>
#include <mochimochi/utility.hpp>

int main() 
{
    // Train command.
    const char* trainCmd = "train";
    const int trainCmdLen = 5;

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

    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Listen to port 9999 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(9999);    // htons is necessary to convert a number to
                                        // network byte order
    if (bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
    {
        std::cout << "Failed to bind to port 9999. errno: " << errno << std::endl;
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

    // Load the hyperparameters
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

    // Read from the connection
    while(1)
    {
        char buffer[100];
        const int bufferLen = 100;
        auto bytesRead = read(connection, buffer, bufferLen);
        std::cout << "Received command: " << buffer;

        if (std::equal(loadCmd, loadCmd+loadCmdLen, buffer))  // Load saved models.
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
        else if (std::equal(saveCmd, saveCmd+saveCmdLen, buffer)) // Save models.
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
        else if (std::equal(exitCmd, exitCmd+exitCmdLen, buffer)) // Exit server loop.
        {
            std::string response = "BYE\n";
            send(connection, response.c_str(), response.size(), 0);
            break;
        }
        else if (std::equal(trainCmd, trainCmd+trainCmdLen, buffer))
        {
            /**
             * Expected command string format:
             *      train <label:int> <pd:float>
             * 
             * e.g.: 
             *  train 1 1.23
             *  train 0 0.32
             **/
            try 
            {
                std::string cmdString(buffer, bufferLen);
                std::istringstream cmdStringStream(cmdString);

                std::vector<std::string> cmdTokens
                {
                    std::istream_iterator<std::string>{cmdStringStream},
                    std::istream_iterator<std::string>{}
                };

                // Parse input string label int and PD float.
                int label = std::stof(cmdTokens[1]);
                float pd = std::stof(cmdTokens[2]);

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

                    // AROW
                    arow_1D.update(data_1D.second, data_1D.first);
                    arow_2D.update(data_2D.second, data_2D.first);
                    arow_3D.update(data_3D.second, data_3D.first);

                    // SCW
                    scw_1D.update(data_1D.second, data_1D.first);
                    scw_2D.update(data_2D.second, data_2D.first);
                    scw_3D.update(data_3D.second, data_3D.first);

                    // NHERD
                    nherd_1D.update(data_1D.second, data_1D.first);
                    nherd_2D.update(data_2D.second, data_2D.first);
                    nherd_3D.update(data_3D.second, data_3D.first);

                    // PA
                    pa_1D.update(data_1D.second, data_1D.first);
                    pa_2D.update(data_2D.second, data_2D.first);
                    pa_3D.update(data_3D.second, data_3D.first);

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
        else if (std::equal(inferCmd, inferCmd+inferCmdLen, buffer))
        {
            /**
             * Expected command string format:
             *      infer <pd:float>
             * 
             * e.g.: 
             *  infer 1.23
             *  infer 0.32
             **/

            try
            {
                std::string cmdString(buffer, bufferLen);
                std::istringstream cmdStringStream(cmdString);

                std::vector<std::string> cmdTokens{
                    std::istream_iterator<std::string>{cmdStringStream}, 
                    std::istream_iterator<std::string>{}
                };

                // Parse input string to float.
                float pd = std::stof(cmdTokens[1]);

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

                // 1D, 2D, and 3D feature spaces input data for inference.
                // The labels, i.e. +1, will be ignore. It's just there because it is expected when calling utility::read_ones().
                std::string line_1D = "+1 1:" + s1;
                std::string line_2D = "+1 1:" + s1 + " 2:" + s2;
                std::string line_3D = "+1 1:" + s1 + " 2:" + s2 + " 3:" + s3;

                // Read data.
                auto data_1D = utility::read_ones<int>(line_1D, 1);
                auto data_2D = utility::read_ones<int>(line_2D, 2);
                auto data_3D = utility::read_ones<int>(line_3D, 3);

                int arow_label_1D = arow_1D.predict(data_1D.second);
                int arow_label_2D = arow_2D.predict(data_2D.second);
                int arow_label_3D = arow_3D.predict(data_3D.second);

                int scw_label_1D = scw_1D.predict(data_1D.second);
                int scw_label_2D = scw_2D.predict(data_2D.second);
                int scw_label_3D = scw_3D.predict(data_3D.second);

                int nherd_label_1D = nherd_1D.predict(data_1D.second);
                int nherd_label_2D = nherd_2D.predict(data_2D.second);
                int nherd_label_3D = nherd_3D.predict(data_3D.second);

                int pa_label_1D = pa_1D.predict(data_1D.second);
                int pa_label_2D = pa_2D.predict(data_2D.second);
                int pa_label_3D = pa_3D.predict(data_3D.second);

                // Inference results formatting 
                std::string arow_inference = "[\n\  {\"AROW\": [" + std::to_string(arow_label_1D) + ", " + std::to_string(arow_label_2D) + ", " + std::to_string(arow_label_3D) + "]},\n"
                    + "  {\"SCW\": [" + std::to_string(scw_label_1D) + ", " + std::to_string(scw_label_2D) + ", " + std::to_string(scw_label_3D) + "]},\n"
                    + "  {\"NHERD\": [" + std::to_string(nherd_label_1D) + ", " + std::to_string(nherd_label_2D) + ", " + std::to_string(nherd_label_3D) + "]},\n"
                    + "  {\"PA\": [" + std::to_string(pa_label_1D) + ", " + std::to_string(pa_label_2D) + ", " + std::to_string(pa_label_3D) + "]}\n"
                    + "]\n";

                send(connection, arow_inference.c_str(), arow_inference.size(), 0);
            }
            catch(const std::exception& e)
            {
                std::cerr << e.what() << '\n';
            } 

            catch (const std::exception& e)
            {
                // TODO: Log error in log file.
                std::string response = "ERROR\n";
                send(connection, response.c_str(), response.size(), 0);
            }
        }
        else
        {
            std::string response = "INVALID\n";
            send(connection, response.c_str(), response.size(), 0);
        }
    }

    // Close the connections
    close(connection);
    close(sockfd);
}
