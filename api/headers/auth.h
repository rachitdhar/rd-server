#ifndef AUTH_H
#define AUTH_H

#include "../headers/sql_handler.h"
#include "../libs/mongoose.h"
#include "../headers/common.h"

char* authenticate(char* user_id, char* passw);
CONTROLLER_RESULT* auth_controller(const char* method, struct mg_http_message* msg);

#endif
