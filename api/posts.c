#include "posts.h"

#define BASE "/api/posts"

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
    snprintf(filepath, MAX_FILE_NAME_LENGTH, "%spost_%s.md", POSTS_PATH, post_id);

    FILE* fp = fopen(filepath, "rb");
    free(filepath);
    if (fp == NULL) return NULL;

    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(filesize);
    if (!buffer) {
	fclose(fp);
	return NULL;
    }

    fread(buffer, 1, filesize, fp);
    fclose(fp);

    STREAM_DATA* sd = malloc(sizeof(STREAM_DATA));
    if (!sd) {
	free(buffer);
	return NULL;
    }
    sd->filesize = filesize;
    sd->buffer = buffer;
    return sd;
}


CONTROLLER_RESULT* posts_controller(const char* method, struct mg_http_message* msg)
{
    CONTROLLER_RESULT* res = malloc(sizeof(CONTROLLER_RESULT));
    if (strcmp(method, "GET") == 0 && mg_match(msg->uri, mg_str(BASE), NULL)) {
	// handling for query params
	char* id = malloc(MAX_FILE_NAME_LENGTH - sizeof("post_"));
	if (mg_http_get_var(&msg->query, "id", id, sizeof(id)) > 0) {
	    res->data = get_post_stream(id);
	    res->type = STREAM;
	    return res;
	}

	res->data = get_posts();
	res->type = JSON;
	return res;
    }
    free(res);
    return NULL;
}
