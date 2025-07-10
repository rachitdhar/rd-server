#include "posts.h"

// get list of all posts
char* get_posts()
{
    sqlite3* db;
    if (sqlite3_open("rdb.db", &db) != SQLITE_OK) {
	sqlite3_close(db);
	return NULL;
    }

    char* response = malloc(MAX_RESPONSE_SIZE);

    const char *query = "select * from posts;";
    sql_select_as_json(db, query, response, MAX_RESPONSE_SIZE);

    sqlite3_close(db);
    return response;
}

// get file stream for post by id
STREAM_DATA* get_post_stream(const char* post_id)
{
    char* filepath = malloc(sizeof(POSTS_PATH) + MAX_FILE_NAME_LENGTH);
    snprintf(filepath, sizeof(filepath), POSTS_PATH, "post_", post_id);
    FILE* fp = fopen(filepath, "rb");
    if (fp == NULL) return NULL;

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(filesize);
    fread(buffer, 1, filesize, fp);
    fclose(fp);

    STREAM_DATA* sd = malloc(sizeof(STREAM_DATA));
    sd->filesize = filesize;
    sd->buffer = buffer;
    return sd;
}


void* posts_controller(const char* method, struct mg_str uri)
{
    if (strcmp(method, "GET") == 0 && mg_match(uri, mg_str(BASE"/"), NULL)) return get_posts();
    if (strcmp(method, "GET") == 0 && mg_match(uri, mg_str(BASE"/#"), NULL)) {
	char* id = malloc(MAX_FILE_NAME_LENGTH - sizeof("post_"));
	mg_http_get_uri_component(uri, 2, id, sizeof(id));
	return get_post_stream(id);
    }
    return NULL;
}
