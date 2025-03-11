#include "interpreter.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>

Interpreter::Interpreter() {
    m_program.resize(0);
    m_memory.fill(0);
    m_instruction_pointer = 0;
    m_memory_pointer = 0;
}

void Interpreter::load_program_from_file(const std::string& program_path) {
    std::ifstream program_file(program_path);

    if (!program_file.is_open())
        throw std::runtime_error("[ERROR] Unable to open the file !");

    char ch;
    std::string program_buffer;
    while (program_file.good()) {
        program_file.get(ch);
        program_buffer.push_back(ch);
    }

    program_file.close();

    m_lexer.fill(program_buffer);

    char c = m_lexer.next();

    while (c) {
        IRInstruction ir_inst;
        switch (c) {
            case '>':
            case '<':
            case '+':
            case '-':
                {
                    char s = m_lexer.next();
                    uint8_t streak = 1;

                    while (c == s) {
                        s = m_lexer.next();
                        streak++;
                    }

                    ir_inst = IRInstruction { static_cast<IRInstructionKind>(c), streak };

                    c = s;
                }
                break;
            case '.':
            case ',':
            case '[':
            case ']':
                ir_inst = IRInstruction { static_cast<IRInstructionKind>(c), {} };
                c = m_lexer.next();
                break;
            default: continue;
        }

        m_program.push_back(ir_inst);
    }
}

void Interpreter::precompute_jumps() {
    std::vector<uint64_t> stack;

    uint64_t local_instruction_pointer = 0;

    while (local_instruction_pointer < m_program.size()) {
        IRInstruction ir_inst = m_program[local_instruction_pointer];

        switch (ir_inst.kind) {
            case IRInstructionKind::JumpIfZero: stack.push_back(local_instruction_pointer); break;
            case IRInstructionKind::JumpIfNotZero:
                {
                    uint64_t target = stack.back();
                    stack.pop_back();
                    m_jump_map[local_instruction_pointer] = target;
                    m_jump_map[target] = local_instruction_pointer;
                }
                break;
            default: break;
        }

        local_instruction_pointer++;
    }
}

void Interpreter::interpret() {
    precompute_jumps();

    while (m_instruction_pointer < m_program.size()) {
        IRInstruction ir_inst = m_program[m_instruction_pointer];

        switch (ir_inst.kind) {
            case IRInstructionKind::IncrementPointer: m_memory_pointer += ir_inst.operand.value(); break;
            case IRInstructionKind::DecrementPointer: m_memory_pointer -= ir_inst.operand.value(); break;
            case IRInstructionKind::IncrementByte: m_memory[m_memory_pointer] += ir_inst.operand.value(); break;
            case IRInstructionKind::DecrementByte: m_memory[m_memory_pointer] -= ir_inst.operand.value(); break;
            case IRInstructionKind::PrintByte:
                {
                    char byte_as_char = static_cast<char>(m_memory[m_memory_pointer]);
                    std::cout << byte_as_char;
                    std::cout.flush();
                }
                break;
            case IRInstructionKind::ReadByte:
                {
                    char input_byte;
                    std::cin >> input_byte;
                    std::cin.clear();
                    m_memory[m_memory_pointer] = static_cast<uint8_t>(input_byte);
                }
                break;
            case IRInstructionKind::JumpIfZero:
                if (m_memory[m_memory_pointer] == 0)
                    m_instruction_pointer = m_jump_map[m_instruction_pointer];
                break;
            case IRInstructionKind::JumpIfNotZero:
                if (m_memory[m_memory_pointer] != 0)
                    m_instruction_pointer = m_jump_map[m_instruction_pointer];
                break;
        }

        m_instruction_pointer++;
    }
}
