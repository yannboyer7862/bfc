#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdint.h>
#include "lexer.h"

#define TOTAL_MEMORY_SIZE 4096

typedef enum {
    INCREMENT_POINTER = '>',
    DECREMENT_POINTER = '<',
    INCREMENT_BYTE = '+',
    DECREMENT_BYTE = '-',
    PRINT_BYTE = '.',
    READ_BYTE = ',',
    JUMP_IF_ZERO = '[',
    JUMP_IF_NOT_ZERO = ']',
} IRInstructionKind;

typedef struct {
    IRInstructionKind kind;
    uint8_t operand;
} IRInstruction;

typedef struct {
    uint8_t memory[TOTAL_MEMORY_SIZE];
    uint64_t instruction_pointer;
    uint64_t memory_pointer;
    IRInstruction* program;
    Lexer lexer;
} Interpreter;

void interpreter_init(Interpreter* interpreter);
void interpreter_load_program_from_file(Interpreter* interpreter, char* program_path);
void interpreter_interpret(Interpreter* interpreter);

#endif
