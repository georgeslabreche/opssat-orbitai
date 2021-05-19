#ifndef CONSTANTS_H_
#define CONSTANTS_H_

// TODO: Figure out if this is enough or too much.
#define COMMAND_BUFFER_LENGTH                                 10000

#define DIR_PATH_LOGS                                         "logs"
#define DIR_PATH_MODELS                                     "models"

#define LOG_FILEPATH_TRAINING                   "logs/training.csv"
#define LOG_FILEPATH_INFERENCE                 "logs/inference.csv"
#define LOG_FILEPATH_ORBITAI                     "logs/orbitai.log"

/* Commands. */
#define COMMAND_RESET                                       "reset"
#define COMMAND_RESET_LENGTH                                      5
#define COMMAND_SAVE                                         "save"
#define COMMAND_SAVE_LENGTH                                       4
#define COMMAND_EXIT                                         "exit"
#define COMMAND_EXIT_LENGTH                                       4

/* Error codes. */
#define NO_ERROR                                                  0
#define ERROR_UNKNOWN                                             1
#define ERROR_PARSE_ARGS                                          2
#define ERROR_INVALID_PORT_NUM                                    3
#define ERROR_CREATE_SOCKET                                       4
#define ERROR_BIND_PORT                                           5
#define ERROR_LISTEN_SOCKET                                       6
#define ERROR_GRAB_CONNECTION                                     7

#endif // CONSTANTS_H_