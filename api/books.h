#ifndef BOOKS_H
#define BOOKS_H

#include "../sql_handler.h"
#include "../mongoose.h"
#include "../common.h"

char* get_books();
CONTROLLER_RESULT* books_controller(const char* method, struct mg_http_message* msg);

#endif
