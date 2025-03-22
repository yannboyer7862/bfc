#include "interpreter.h"
#include "lexer.h"
#include <stdio.h>

#include "stb_ds.h"

void interpreter_init(Interpreter* interpreter) {
    for (size_t i = 0; i < TOTAL_MEMORY_SIZE; i++) {
        interpreter->memory[i] = 0;
    }

    interpreter->instruction_pointer = 0;
    interpreter->memory_pointer = 0;
    interpreter->program = NULL;
    lexer_init(&interpreter->lexer);
}

void interpreter_load_program_from_file(Interpreter* interpreter, char* program_path) {
    FILE* program_file = fopen(program_path, "r");

    if (program_file == NULL) {
        printf("[ERROR] Unable to open the file !\n");
        exit(EXIT_FAILURE);
    }

    char* program_buffer = NULL;
    char ch;
    while ((ch = fgetc(program_file)) != EOF) {
        arrpush(program_buffer, ch);
    }

    lexer_fill(&interpreter->lexer, program_buffer);

    fclose(program_file);

    arrfree(program_buffer);

    char c = lexer_next(&interpreter->lexer);

    while (c) {
        IRInstruction ir_inst;
        switch (c) {
            case '>':
            case '<':
            case '+':
            case '-':
            case '.':
            case ',':
                {
                    char s = lexer_next(&interpreter->lexer);
                    uint8_t streak = 1;

                    while (c == s) {
                        streak++;
                        s = lexer_next(&interpreter->lexer);
                    }

                    ir_inst = (IRInstruction) { .kind = c, .operand = streak };

                    c = s;
                }
                break;
            case '[':
            case ']':
                {
                    ir_inst = (IRInstruction) { .kind = c, .operand = 0 };
                    c = lexer_next(&interpreter->lexer);
                }
                break;
            default: continue;
        }

        arrpush(interpreter->program, ir_inst);
    }

    lexer_free(&interpreter->lexer);
}

void interpreter_interpret(Interpreter* interpreter) {

    uint64_t* stack = NULL;
    uint64_t targets[arrlen(interpreter->program)];

    for (uint64_t i = 0, j; i < arrlen(interpreter->program); i++) {
        if (interpreter->program[i].kind == JUMP_IF_ZERO) {
            arrpush(stack, i);
        } else if (interpreter->program[i].kind == JUMP_IF_NOT_ZERO) {
            if (arrlen(stack) == 0) {
                printf("Unmatched ']' at byte %lu", (i + 1));
                exit(EXIT_FAILURE);
            } else {
                j = arrpop(stack);
                targets[i] = j;
                targets[j] = i;
            }
        }
    }

    arrfree(stack);

    while (interpreter->instruction_pointer < arrlen(interpreter->program)) {
        IRInstruction ir_inst = interpreter->program[interpreter->instruction_pointer];

        switch (ir_inst.kind) {
            case INCREMENT_POINTER: interpreter->memory_pointer += ir_inst.operand; break;
            case DECREMENT_POINTER: interpreter->memory_pointer -= ir_inst.operand; break;
            case INCREMENT_BYTE: interpreter->memory[interpreter->memory_pointer] += ir_inst.operand; break;
            case DECREMENT_BYTE: interpreter->memory[interpreter->memory_pointer] -= ir_inst.operand; break;
            case PRINT_BYTE:
                {
                    for (size_t i = 0; i < ir_inst.operand; i++) {
                        char byte_as_char = (char)interpreter->memory[interpreter->memory_pointer];
                        printf("%c", byte_as_char);
                        fflush(stdout);
                    }
                }
                break;
            case READ_BYTE:
                {
                    for (size_t i = 0; i < ir_inst.operand; i++) {
                        char input_byte;
                        int check = scanf("%c", &input_byte);
                        if (check < 0) {
                            printf("[ERROR] Your input is wrong\n");
                            exit(EXIT_FAILURE);
                        }
                        interpreter->memory[interpreter->memory_pointer] = (uint8_t)input_byte;
                    }
                }
                break;
            case JUMP_IF_ZERO:
                if (interpreter->memory[interpreter->memory_pointer] == 0)
                    interpreter->instruction_pointer = targets[interpreter->instruction_pointer];
                break;
            case JUMP_IF_NOT_ZERO:
                if (interpreter->memory[interpreter->memory_pointer] != 0)
                    interpreter->instruction_pointer = targets[interpreter->instruction_pointer];
                break;
        }

        interpreter->instruction_pointer++;
    }

    arrfree(interpreter->program);
}
