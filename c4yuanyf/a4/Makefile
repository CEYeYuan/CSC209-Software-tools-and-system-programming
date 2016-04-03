Port=54248
CFLAGS= -DPORT=\$(PORT) -g -Wall -Werror -std=c99

all:friend
friend: friendme.c friends.h friends.c
	gcc CFLAGS -o $@ $^ 
