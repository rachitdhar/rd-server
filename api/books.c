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

    const char *query =
    "select"
    " b.id, b.name, b.author_name, bs.code as status_code, bs.name as status, bc.code as classification_code, bc.name as classification"
    " from books b"
    " inner join book_status bs on bs.id = b.status_id"
    " inner join book_classification bc on bc.id = b.classification_id;";

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
