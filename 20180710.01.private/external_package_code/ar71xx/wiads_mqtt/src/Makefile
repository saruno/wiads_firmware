.PHONY: build, clean, run, test

CFLAGS += -Wall #-Werror
LDFLAGS += -lmosquitto

# Uncomment this to print out debugging info.
CFLAGS += -DDEBUG

PROJECT=wiads_mqtt
CC = gcc
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(wildcard *.h)

# build httpjson executable when user executes "make"
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@

$(PROJECT): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $(PROJECT)

build: $(PROJECT)

run: ./$(PROJECT)

test: build run

# remove object files and executable when user executes "make clean"
clean:
	rm *.o $(PROJECT)
