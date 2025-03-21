#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("[ERROR] No program provided !\n");
        printf("[INFO] Usage : ./bfc program.bf\n");
        return EXIT_FAILURE;
    }

    Interpreter my_interpreter;

    interpreter_init(&my_interpreter);

    char* program_path = argv[1];

    interpreter_load_program_from_file(&my_interpreter, program_path);

    interpreter_interpret(&my_interpreter);

    return EXIT_SUCCESS;
}
