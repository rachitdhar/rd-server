#define MAX_JSON_RESPONSE_SIZE 100000
#define MAX_JSON_DATA_SIZE 99500
#define MAX_JSON_MSG_SIZE 500

#define JSON_CONTENT_TYPE "Content-Type: application/json\r\n"
#define STREAM_CONTENT_TYPE "Content-Type: text/plain\r\nContent-Length: %ld\r\n\r\n"
#define INTERNAL_SERVER_ERROR_JSON "{ \"status\": 500, \"data\": \"\", \"message\": \"Internal server error\" }"

typedef struct {
    char* buffer;
    long filesize;
} STREAM_DATA;

typedef enum {
    JSON,
    STREAM
} RESPONSE_TYPES;

typedef struct {
    int status_code;
    void* data;
    char* message;
    RESPONSE_TYPES type;
} HTTP_RESPONSE_DETAILS;
