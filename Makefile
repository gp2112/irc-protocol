
CC = gcc
SRC = ./src
DEPS = $(SRC)/queue.c $(SRC)/irc.c $(SRC)/buffer.c $(SRC)/interface.c

FLAGS = -g
LIBS = -lncurses -lpthread

all:
	make clear
	make server
	make client

server:
	$(CC) $(SRC)/server.c $(DEPS) $(FLAGS) $(LIBS) -o server

client:
	$(CC) $(SRC)/client.c $(DEPS) $(FLAGS) $(LIBS) -o client

clear:
ifneq (,$(wildcard server))
	rm server
endif
ifneq (,$(wildcard client))
	rm client
endif
