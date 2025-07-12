#include "sql_handler.h"

// execute a select query and return the JSON response
// (the address of a buffer is passed as a parameter to which the response will be written)
void sql_select_as_json(sqlite3* db, const char* query, char* buffer, size_t buffer_size)
{
    sqlite3_stmt *s;
    int res = sqlite3_prepare_v2(db, query, -1, &s, NULL);
    if (res != SQLITE_OK) return;

    int offset = 0;
    offset += snprintf(buffer + offset, buffer_size - offset, "[");

    int row_count = 0;
    while ((res = sqlite3_step(s)) == SQLITE_ROW) {
        int col_count = sqlite3_column_count(s);
        if (row_count > 0) {
	    offset += snprintf(buffer + offset, buffer_size - offset, ",");
	}
        offset += snprintf(buffer + offset, buffer_size - offset, "{");

        for (int i = 0; i < col_count; i++) {
            const char *col_name = sqlite3_column_name(s, i);
            const char *col_text = (const char *)sqlite3_column_text(s, i);

	    offset += snprintf(buffer + offset, buffer_size - offset,
	    "\"%s\": \"%s\"%s",
	    col_name,
	    col_text ? col_text : "",
	    (i < col_count - 1) ? "," : "");
        }
	offset += snprintf(buffer + offset, buffer_size - offset, "}");
        row_count++;
    }
    offset += snprintf(buffer + offset, buffer_size - offset, "]");

    sqlite3_finalize(s);
}
