#include "mongoose.h"
#include <stdio.h>

#include "api/books.h"

#define URL "http://0.0.0.0:8000"
#define MAX_JSON_RESPONSE_SIZE 100000
#define MAX_JSON_DATA_SIZE 99500
#define MAX_JSON_MSG_SIZE 500

typedef struct {
    int status_code;
    char* data;
    char* message;
} HTTP_RESPONSE_DETAILS;

static char response[MAX_JSON_RESPONSE_SIZE];


void construct_json_response(HTTP_RESPONSE_DETAILS* hrd)
{
    snprintf(
        response, sizeof(response),
        "{ \"status\": %d, \"data\": \"%s\", \"message\": \"%s\" }",
        hrd->status_code, hrd->data, hrd->message
    );
}


char* get_method_type(struct mg_str method)
{
    if (mg_strcmp(method, mg_str("GET")) == 0) return "GET";
    if (mg_strcmp(method, mg_str("POST")) == 0) return "POST";
    if (mg_strcmp(method, mg_str("PUT")) == 0) return "PUT";
    if (mg_strcmp(method, mg_str("DELETE")) == 0) return "DELETE";
}


HTTP_RESPONSE_DETAILS* get_api_endpoint_response(const char* method, struct mg_str uri)
{
    HTTP_RESPONSE_DETAILS* hrd = malloc(sizeof(HTTP_RESPONSE_DETAILS));
    if (!hrd) return NULL;

    hrd->message = malloc(MAX_JSON_MSG_SIZE);
    if (!hrd->message) return NULL;

    if (mg_match(uri, mg_str("/api/books/"), NULL)) {
	void* result = books_controller(method, uri);
	if (!result) {
	    hrd->status_code = 500;
	    hrd->data = "";
	    hrd->message = "Books endpoint failed";
	} else {
	    hrd->status_code = 200;
	    hrd->data = (const char*)result;
	    hrd->message = "Message request successful";
	}
    } else {
	hrd->status_code = 404;
	hrd->data = "";
	hrd->message = "Failed due to invalid uri";
    }
    return hrd;
}


void send_default_failure_response(struct mg_connection *conn)
{
    mg_http_reply(
        conn,
        500,
        "Content-Type: application/json\r\n",
        "%s",
        "{ \"status\": 500, \"data\": \"\", \"message\": \"Internal server error\" }"
    );
}


static void event_handler(struct mg_connection *conn, int event, void* event_data)
{
    if (event != MG_EV_HTTP_MSG) return;

    struct mg_http_message *msg = (struct mg_http_message*) event_data;
    const char* method = get_method_type(msg->method);

    HTTP_RESPONSE_DETAILS* hrd = get_api_endpoint_response(method, msg->uri);
    if (!hrd) {
	send_default_failure_response(conn);
	return;
    }
    construct_json_response(hrd);
    if (!response) {
	send_default_failure_response(conn);
	free(hrd->data);
	free(hrd->message);
	free(hrd);
	return;
    }

    mg_http_reply(conn, hrd->status_code, "Content-Type: application/json\r\n", "%s", response);

    free(hrd->data);
    free(hrd->message);
    free(hrd);
}


int main(void)
{
    struct mg_mgr event_manager;
    mg_mgr_init(&event_manager);

    mg_http_listen(&event_manager, URL, event_handler, NULL); // start HTTP server on a port
    printf("Server listening on PORT 8000...\n");

    while (1) mg_mgr_poll(&event_manager, 1000); // event loop

    mg_mgr_free(&event_manager);
    return 0;
}
