CFLAGS=-std=c99 -Wall -g -Werror
all: mapreduce mapworker reduceworker benchmark

mapreduce: master.c safe.c safe.h 
	gcc ${CFLAGS} -o $@ $^

mapworker: mapworker.c safe.c safe.h mapreduce.h word_freq.c
	gcc ${CFLAGS} -o $@ $^

reduceworker: reduceworker.c safe.c safe.h mapreduce.h word_freq.c linkedlist.h linkedlist.c
	gcc ${CFLAGS} -o $@ $^

benchmark: benchmark.c safe.c safe.h mapreduce.h word_freq.c linkedlist.h linkedlist.c
	gcc ${CFLAGS} -o $@ $^