CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

<<<<<<< HEAD
assemble: assemble.o parse-asm.o assemble-rw.o branch-asm.o directives.o alias.o instr-utils.o symbol-table.o
=======
assemble: assemble.o assemble-rw.o parse-asm.o directives.o branch-asm.o symbol-table.o instr-utils.o
>>>>>>> f37220193c0c1faaba7661a60fbf360a722079fc
emulate: emulate.o cpu.o loader.o dp-imm.o dp-reg.o execute.o branch.o transfer.o arithmetic-op.o instr-utils.o

clean:
	$(RM) *.o assemble emulate cpu loader dp-imm dp-reg execute branch transfer arithmetic-op instr-utils
	
