## Compile target toyasm (ToyASM)
bin_PROGRAMS=toyasm

## ToyASM target source and header files. Headers go at the bottom of the list!
toyasm_SOURCES=\
src/codelist.cpp \
src/input_tokenizer.cpp \
src/instr_list.cpp \
src/instruction.cpp \
src/toyasm.cpp \
src/main.cpp \
include/asm.h \
include/instr_list.h

## extra compiler flags
toyasm_CXXFLAGS=-I./include

EXTRA_DIST=m4/NOTES