#ifndef SQL_HANDLER_H
#define SQL_HANDLER_H

#include <stdio.h>
#include <string.h>
#include "sqlite3.h"

#define MAX_RESPONSE_SIZE 99000

void sql_select_as_json(sqlite3* db, const char* query, char* buffer, size_t buffer_size);

#endif
