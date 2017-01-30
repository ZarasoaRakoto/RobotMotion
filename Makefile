TARGET = 
SOURCE_DIR = sources
SOURCES = $(SOURCE_DIR)/containers.c
OBJS = $(SOURCES:.c=.o)
CC = gcc
CFLAGS = -Wall -Werror -std=gnu99
BASE_HEADERS = $(SOURCE_DIR)/stdclibs.h
PREFIX =
C_LIB_DIR =
C_LIB = 

INCLUDE_DIR = 
TEST_SOURCES = $(SOURCES)
TEST_OBJS = $(TEST_SOURCES:.c=.o)
TEST_EXEC_NAME = 

all: test

%.o:  %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS) $(BASE_HEADERS)
	$(CC) $(CFLAGS) $^ -o $@
	rm -f $(SOURCE_DIR)/*.o

clean: 
	rm -f $(TARGET) $(OBJS) $(TEST_OBJS)

test: $(TEST_OBJS) $(BASE_HEADERS)
	#$(CC) $(CFLAGS) $^ -o $@
	#rm -f $(SOURCE_DIR)/*.o

.PHONY: clean all
