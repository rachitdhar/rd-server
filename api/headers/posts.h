#ifndef POSTS_H
#define POSTS_H

#include "../sql_handler.h"
#include "../mongoose.h"
#include "../common.h"

char* get_posts();
STREAM_DATA* get_post_stream(const char* post_id);
CONTROLLER_RESULT* posts_controller(const char* method, struct mg_http_message* msg);

#endif
