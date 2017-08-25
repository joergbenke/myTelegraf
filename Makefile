#!/bin/bash

CC=gcc
CFLAGS=-std=c99 -ggdb

all: memory memory_with_argument

memory: memory.c
	${CC} ${CFLAGS} -o memory memory.c

memory_with_argument: memory_with_argument.c
	${CC} ${CFLAGS} -o memory_with_argument memory_with_argument.c 

clean: 
	rm memory memory_with_argument *~
