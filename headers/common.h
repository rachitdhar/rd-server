#ifndef COMMON_H
#define COMMON_H

#define MAX_JSON_RESPONSE_SIZE 100000
#define MAX_JSON_DATA_SIZE 99500
#define MAX_JSON_MSG_SIZE 500
#define MAX_FILE_NAME_LENGTH 100

#define CORS_HEADERS "Access-Control-Allow-Origin: https://rachitdhar.github.io\r\nAccess-Control-Allow-Methods: GET, POST, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type, ngrok-skip-browser-warning\r\n"
#define JSON_CONTENT_TYPE "Content-Type: application/json\r\n"
#define INTERNAL_SERVER_ERROR_JSON "{ \"status\": 500, \"data\": \"\", \"message\": \"Internal server error\" }"

// NOTE: fopen() does not care about relative path, it just starts from the project root
#define POSTS_PATH "store/posts/"

typedef struct {
    char* buffer;
    long filesize;
} STREAM_DATA;

typedef enum {
    JSON,
    STREAM
} RESPONSE_TYPE;

typedef struct {
    int status_code;
    void* data;
    char* message;
    RESPONSE_TYPE type;
} HTTP_RESPONSE_DETAILS;

typedef struct {
    void* data;
    RESPONSE_TYPE type;
} CONTROLLER_RESULT;


// to get the stream contents from a file at a given path
STREAM_DATA* get_file_stream(char* filepath);

#endif
