#include "mongoose.h"
#include <stdio.h>
#include "common.h"

#include "api/books.h"
#include "api/posts.h"

#define URL "http://0.0.0.0:8000"

static char response[MAX_JSON_RESPONSE_SIZE];


int construct_json_response(HTTP_RESPONSE_DETAILS* hrd)
{
    return snprintf(
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
    return NULL;
}


void handle_controller_result(
CONTROLLER_RESULT* (*controller)(const char*, struct mg_http_message*),
const char* method,
struct mg_http_message* msg,
HTTP_RESPONSE_DETAILS* hrd
) {
    CONTROLLER_RESULT* result = (*controller)(method, msg);
    if (!result) {
	hrd->status_code = 500;
	hrd->data = "";
	hrd->message = "API endpoint failed";
	hrd->type = JSON;
    } else {
	hrd->status_code = 200;
	hrd->data = result->data;
	hrd->message = "Message request successful";
	hrd->type = result->type;
    }
}


HTTP_RESPONSE_DETAILS* get_api_endpoint_response(const char* method, struct mg_http_message* msg)
{
    HTTP_RESPONSE_DETAILS* hrd = malloc(sizeof(HTTP_RESPONSE_DETAILS));
    if (!hrd) return NULL;

    hrd->message = malloc(MAX_JSON_MSG_SIZE);
    if (!hrd->message) return NULL;

    if (mg_match(msg->uri, mg_str("/api/books"), NULL)) handle_controller_result(&books_controller, method, msg, hrd);
    else if (mg_match(msg->uri, mg_str("/api/posts"), NULL)) handle_controller_result(&posts_controller, method, msg, hrd);
    else {
	hrd->status_code = 404;
	hrd->data = "";
	hrd->message = "Failed due to invalid uri";
	hrd->type = JSON;
    }
    return hrd;
}


void send_default_failure_response(struct mg_connection *conn)
{
    mg_http_reply(
        conn,
        500,
        JSON_CONTENT_TYPE,
        "%s",
        INTERNAL_SERVER_ERROR_JSON
    );
}


static void event_handler(struct mg_connection *conn, int event, void* event_data)
{
    if (event != MG_EV_HTTP_MSG) return;

    struct mg_http_message *msg = (struct mg_http_message*) event_data;
    const char* method = get_method_type(msg->method);

    HTTP_RESPONSE_DETAILS* hrd = get_api_endpoint_response(method, msg);
    if (!hrd) {
	send_default_failure_response(conn);
	free(hrd);
	return;
    }

    if (hrd->type == JSON) {
	int construction_result = construct_json_response(hrd);
	if (construction_result <= 0) {
	    send_default_failure_response(conn);
	    free(hrd->data);
	    free(hrd->message);
	    free(hrd);
	    return;
	}

	mg_http_reply(conn, hrd->status_code, JSON_CONTENT_TYPE, "%s", (char*)response);
    } else if (hrd->type == STREAM) {
	STREAM_DATA* sd = (STREAM_DATA*)hrd->data;

	mg_printf(conn,
        "HTTP/1.1 %d OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %ld\r\n\r\n",
        hrd->status_code, sd->filesize); // sending header

	mg_send(conn, sd->buffer, sd->filesize); // sending file stream
	conn->is_draining = 1; // to close the connection
    }

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
