#include <iostream>
#include <cstdlib>
#include <exception>
#include "interpreter.hpp"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cerr << "[ERROR] No program provided !\n";
        std::cerr << "[ERROR] Usage : ./bfc my_program.bf\n";
        return EXIT_SUCCESS;
    }

    std::string program_path = std::string(argv[1]);

    Interpreter my_interpreter;

    try {
        my_interpreter.load_program_from_file(program_path);
    } catch (std::exception& e) {
        std::cerr << e.what() << "\n";
        return EXIT_FAILURE;
    }

    my_interpreter.interpret();

    return EXIT_SUCCESS;
}
