#include "mongoose.h"
#include <stdio.h>
#include "common.h"

#include "api/books.h"
#include "api/posts.h"

#define URL "http://0.0.0.0:5000"

static char response[MAX_JSON_RESPONSE_SIZE];


int construct_json_response(HTTP_RESPONSE_DETAILS* hrd)
{
    return snprintf(
        response, sizeof(response),
        "{ \"status\": %d, \"data\": %s, \"message\": \"%s\" }",
        hrd->status_code, (char*)(hrd->data), hrd->message
    );
}


char* get_method_type(struct mg_str method)
{
    if (mg_strcmp(method, mg_str("GET")) == 0) return "GET";
    if (mg_strcmp(method, mg_str("POST")) == 0) return "POST";
    if (mg_strcmp(method, mg_str("PUT")) == 0) return "PUT";
    if (mg_strcmp(method, mg_str("DELETE")) == 0) return "DELETE";
    if (mg_strcmp(method, mg_str("OPTIONS")) == 0) return "OPTIONS";
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


bool uri_has_prefix(struct mg_str *uri, const char *prefix) {
    size_t len = strlen(prefix);
    return uri->len >= len &&
           strncmp(uri->buf, prefix, len) == 0 &&
           (uri->len == len || uri->buf[len] == '/' || uri->buf[len] == '\0');
}


HTTP_RESPONSE_DETAILS* get_api_endpoint_response(const char* method, struct mg_http_message* msg)
{
    HTTP_RESPONSE_DETAILS* hrd = malloc(sizeof(HTTP_RESPONSE_DETAILS));
    if (!hrd) return NULL;

    if (uri_has_prefix(&msg->uri, "/api/books")) handle_controller_result(&books_controller, method, msg, hrd);
    else if (uri_has_prefix(&msg->uri, "/api/posts")) handle_controller_result(&posts_controller, method, msg, hrd);
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

    // to handle CORS
    if (strcmp(method, "OPTIONS") == 0) {
	mg_http_reply(conn, 200,
	"Access-Control-Allow-Origin: https://rachitdhar.github.io\r\n"
	"Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
	"Access-Control-Allow-Headers: Content-Type, ngrok-skip-browser-warning\r\n",
	"");
	return;
    }

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
	    free(hrd);
	    return;
	}

	mg_http_reply(conn, hrd->status_code, CORS_HEADERS JSON_CONTENT_TYPE, "%s", (char*)response);
    } else if (hrd->type == STREAM) {
	STREAM_DATA* sd = (STREAM_DATA*)hrd->data;

	mg_printf(conn,
        "HTTP/1.1 %d OK\r\n"
	CORS_HEADERS
	"Content-Type: text/plain\r\nContent-Length: %ld\r\n\r\n",
        hrd->status_code, sd->filesize); // sending header

	mg_send(conn, sd->buffer, sd->filesize); // sending file stream
	conn->is_draining = 1; // to close the connection
    }

    free(hrd->data);
    free(hrd);
}


int main(void)
{
    struct mg_mgr event_manager;
    mg_mgr_init(&event_manager);

    mg_http_listen(&event_manager, URL, event_handler, NULL); // start HTTP server on a port
    printf("Server listening on PORT 5000...\n");

    while (1) mg_mgr_poll(&event_manager, 1000); // event loop

    mg_mgr_free(&event_manager);
    return 0;
}
