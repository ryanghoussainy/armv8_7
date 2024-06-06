CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o parse-asm.o assemble-rw.o branch-asm.o directives.o instr-utils.o symbol-table.o dp-imm-assemble.o dp-reg-assemble.o transfer-assemble.o
emulate: emulate.o cpu.o loader.o dp-imm.o dp-reg.o execute.o branch.o transfer.o arithmetic-op.o instr-utils.o

clean:
	$(RM) *.o assemble emulate cpu loader dp-imm dp-reg execute branch transfer arithmetic-op instr-utils
	
