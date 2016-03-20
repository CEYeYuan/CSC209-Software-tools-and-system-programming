CFLAGS=-std=c99 -Wall -Werror
all: mapreduce mapworker

mapreduce: master.c safe.c safe.h
	gcc ${CFLAGS} -o $@ $^

mapworker: mapworker.c safe.c safe.h mapreduce.h word_freq.c
	gcc ${CFLAGS} -o $@ $^
