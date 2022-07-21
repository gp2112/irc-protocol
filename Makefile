
CC = gcc
SRC = ./src
DEPS = ./server/channel.c ./server/client.c ./server/controller.c ./server/datetime.c ./server/logger.c ./server/queue.c ./server/server.c ./server/signalhandler.c

FLAGS = -g
LIBS = -lpthread

all:
	make clear
	make server
	make client

serv:
	$(CC) $(DEPS) $(FLAGS) $(LIBS) -o server2

client:
	$(CC) $(SRC)/client.c $(DEPS) $(FLAGS) $(LIBS) -o client

clear:
ifneq (,$(wildcard server))
	rm server
endif
ifneq (,$(wildcard client))
	rm client
endif
