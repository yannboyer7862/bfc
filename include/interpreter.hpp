#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <string>
#include <cstdint>
#include <array>
#include <vector>
#include <map>
#include <optional>
#include "lexer.hpp"

enum class IRInstructionKind {
    IncrementPointer = '>',
    DecrementPointer = '<',
    IncrementByte = '+',
    DecrementByte = '-',
    PrintByte = '.',
    ReadByte = ',',
    JumpIfZero = '[',
    JumpIfNotZero = ']',
};

struct IRInstruction {
    IRInstructionKind kind;
    std::optional<uint8_t> operand;
};

class Interpreter {
    public:
        Interpreter();

        void load_program_from_file(const std::string& program_path);
        void interpret();
    private:
        static constexpr uint32_t TOTAL_MEMORY_SIZE = 100000;
        void precompute_jumps();

        std::vector<IRInstruction> m_program;
        std::array<uint8_t, TOTAL_MEMORY_SIZE> m_memory;
        std::map<uint64_t, uint64_t> m_jump_map;

        uint64_t m_instruction_pointer;
        uint64_t m_memory_pointer;

        Lexer m_lexer;
};

#endif
