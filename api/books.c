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


void* books_controller(const char* method, struct mg_str uri)
{
    if (strcmp(method, "GET") == 0 && mg_match(uri, mg_str(BASE"/"), NULL)) return get_books();
    return NULL;
}
