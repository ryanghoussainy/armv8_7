CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
emulate: emulate.o cpu.o loader.o masks.o dp-imm.o dp-reg.o execute.o branch.o transfer.o arithmetic-op.o

clean:
	$(RM) *.o assemble emulate cpu loader masks dp-imm dp-reg execute branch transfer arithmetic-op
	
