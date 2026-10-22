
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2 -Iinclude
PYTHON = python

TARGET = hull.exe
ALGORITHM ?= andrews
SOURCES = main.c $(wildcard src/*.c)

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	.\$(TARGET)
	$(PYTHON) visualize.py $(ALGORITHM)

clean:
	del /Q $(TARGET) hull_data.txt 2>NUL

.PHONY: all run clean
