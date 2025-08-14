#ifndef BOOKS_H
#define BOOKS_H

#include "../../headers/sql_handler.h"
#include "../../libs/mongoose.h"
#include "../../headers/common.h"

char* get_books();
CONTROLLER_RESULT* books_controller(const char* method, struct mg_http_message* msg);

#endif
