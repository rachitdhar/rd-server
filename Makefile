CC = gcc
NAME = server
SRCS = source.c sql_handler.c mongoose.o sqlite3.o api/books.c
CFLAGS = -Wall -Wextra -D_POSIX_C_SOURCE=200000L
LDFLAGS = -lws2_32

all:
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME) $(LDFLAGS) && ./$(NAME)
	@echo -e "\n"
	rm -f $(NAME)

compile-libs:
	$(CC) -c -D_POSIX_C_SOURCE=200000L mongoose.c -o mongoose.o
	$(CC) -c sqlite3.c -o sqlite3.o

build:
	$(CC) $(CFLAGS) $(SRCS) -o $(NAME) $(LDFLAGS)

run:
	./$(NAME)

clear:
	rm -f $(NAME)
