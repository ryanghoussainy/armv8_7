CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: shell

shell: files.o linked-list.o shell.o ls.o instruction.o

clean:
	$(RM) *.o