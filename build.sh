#!/bin/sh

PROG=play_emotion
CC=gcc
DEPEND_LIBS="ecore ecore-evas ecore-input emotion evas"
CFLAGS=
LIBS=
for i in ${DEPEND_LIBS} ;
do
    CFLAGS="${CFLAGS} `pkg-config --cflags $i`"
    LIBS="${LIBS} `pkg-config --libs $i`"
done

echo "${CC} ${CFLAGS} main.c ${LIBS}"
${CC} ${CFLAGS} -o ${PROG} main.c ${LIBS}






