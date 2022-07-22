
CC = gcc
SRC = ./src
DEPS = ./server/channel.c ./server/client.c ./server/controller.c ./server/datetime.c ./server/logger.c ./server/queue.c ./server/server.c ./server/main.c

FLAGS = -g -Wno-error=format-security
LIBS = -lpthread

all:
	make clear
	make server
	make client

serv:
	$(CC) ./server/*.c $(FLAGS) $(LIBS) -o server2

cli:
	$(CC) ./client/*.c $(FLAGS) $(LIBS) -lncurses -o clirc

clear:
ifneq (,$(wildcard server))
	rm server
endif
ifneq (,$(wildcard client))
	rm client
endif
