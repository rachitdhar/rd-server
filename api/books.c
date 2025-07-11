#include "books.h"

#define BASE "/api/books"

// get list of all books
char* get_books()
{
    sqlite3* db;
    if (sqlite3_open("rdb.db", &db) != SQLITE_OK) {
	sqlite3_close(db);
	return NULL;
    }

    char* response = malloc(MAX_RESPONSE_SIZE);

    const char *query = "select * from books;";
    sql_select_as_json(db, query, response, MAX_RESPONSE_SIZE);

    sqlite3_close(db);
    return response;
}


CONTROLLER_RESULT* books_controller(const char* method, struct mg_http_message* msg)
{
    CONTROLLER_RESULT* res = malloc(sizeof(CONTROLLER_RESULT));
    if (strcmp(method, "GET") == 0 && mg_match(msg->uri, mg_str(BASE), NULL)) {
	res->data = get_books();
	res->type = JSON;
	return res;
    }
    free(res);
    return NULL;
}
