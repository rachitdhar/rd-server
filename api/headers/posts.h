#ifndef POSTS_H
#define POSTS_H

#include "../../headers/sql_handler.h"
#include "../../libs/mongoose.h"
#include "../../headers/common.h"

char* get_posts();
STREAM_DATA* get_post_stream(const char* post_id);
CONTROLLER_RESULT* posts_controller(const char* method, struct mg_http_message* msg);

#endif
