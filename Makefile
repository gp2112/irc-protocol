
CC = gcc
DEPS = queue.c irc.c buffer.c interface.c
FLAGS = -g
LIBS = -lncurses -lpthread

all:
	make clear
	make server
	make client

server:
	$(CC) server.c $(DEPS) $(FLAGS) $(LIBS) -o server

client:
	$(CC) client.c $(DEPS) $(FLAGS) $(LIBS) -o client

clear:
ifneq (,$(wildcard server))
	rm server
endif
ifneq (,$(wildcard client))
	rm client
endif
