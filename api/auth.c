#include "headers/auth.h"

#define BASE "/api/auth"
#define MAX_QUERY_SIZE 200

// validate the user and password, and return the auth token
char* authenticate(char* user_id, char* passw)
{
    // get the hash of passw
    char hash[SHA256_HASH_SIZE];
    get_sha256(passw, hash);

    // retrieve the hash for this user (if the user_id exists)
    char* response = malloc(MAX_RESPONSE_SIZE);
    char* query = malloc(MAX_QUERY_SIZE);

    snprintf(query, MAX_QUERY_SIZE,
    "select"
    " u.hash"
    " from users u"
    " where u.user_id = %s"
    " and u.is_admin = 1",
    user_id);

    sqlite3* db;
    if (sqlite3_open("rdb.db", &db) != SQLITE_OK) {
	sqlite3_close(db);
	return NULL;
    }

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(db, query, -1, &stmt, NULL) != SQLITE_OK) {
	sqlite3_close(db);
	return NULL;
    }

    int rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	return NULL;
    }
    char* out = malloc(SHA256_HASH_SIZE);
    snprintf(out, sizeof(out), "%s", sqlite3_column_text(stmt, 0));
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // validate the retrieved hash against the hash calculated
    if (strcmp(hash, out) != 0) return NULL;
    free(out);

    // generate a token and return it
    //return generate_jwt();
    return "abcd1234"; // TODO: Handle JWT token generation
}

CONTROLLER_RESULT* auth_controller(const char* method, struct mg_http_message* msg)
{
    CONTROLLER_RESULT* res = malloc(sizeof(CONTROLLER_RESULT));
    if (strcmp(method, "POST") == 0 && mg_match(msg->uri, mg_str(BASE), NULL)) {
	char user_id[100], passw[100];
	mg_json_get(msg->body, "$.userid", user_id, sizeof(username));
	mg_json_get(msg->body, "$.password", passw, sizeof(password));

	res->token = authenticate(user_id, passw);
	res->data = "";
	res->type = JSON;
	return res;
    }
    free(res);
    return NULL;
}
