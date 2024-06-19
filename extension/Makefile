CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: shell

shell: files.o linked-list.o shell.o command.o commands/ls.o commands/cd.o commands/pwd.o commands/rmdir.o commands/mkdir.o commands/rm.o commands/touch.o commands/man.o commands/echo.o commands/cat.o

clean:
	$(RM) *.o shell commands/*.o