# makefile for compression project "Coder"

CC=g++
FLAGS=-Wall -c

OUT=bin/coder
FILES=../*.cpp ../Fatal/*.cpp ../Codec/*.cpp

all:	link clean_temporals

link:	compile
	-mkdir bin
	$(CC) obj/* -o $(OUT)

compile:
	-mkdir obj
	cd obj;\
	$(CC) $(FLAGS) $(FILES);\
	cd ..

clean_temporals:
	-rm obj/*
	-rmdir obj

clean:	clean_temporals
	-rm bin/*
	-rmdir bin
