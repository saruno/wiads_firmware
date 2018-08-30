.PHONY: build, clean, run, test

PROJECT = wiads_daemon

CC = gcc
CFLAGS = -Wall
LDFLAGS = -s
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
