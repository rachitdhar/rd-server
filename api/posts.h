#include "../sql_handler.h"
#include "../mongoose.h"
#include "../common.h"

#define BASE "/api/posts"
#define POSTS_PATH "../store/posts/"
#define MAX_FILE_NAME_LENGTH 100

char* get_posts();
void* posts_controller(const char* method, struct mg_str uri);
