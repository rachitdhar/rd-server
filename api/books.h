#include "../sql_handler.h"
#include "../mongoose.h"

char* get_books();
void* books_controller(const char* method, struct mg_str uri);
