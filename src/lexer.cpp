#include "lexer.hpp"

Lexer::Lexer() {
    m_code.resize(0);
    m_position_in_code = 0;
}

void Lexer::fill(const std::string& code) {
    for (std::size_t i = 0; i < code.size(); i++) {
        m_code.push_back(code[i]);
    }
}

bool Lexer::is_valid_instruction(char instruction) {
    std::string valid_insts = "><+-.,[]";
    if (valid_insts.find(instruction) != std::string::npos)
        return true;
    else
        return false;
}

char Lexer::next() {
    while (m_position_in_code < m_code.size() && !is_valid_instruction(m_code[m_position_in_code]))
        m_position_in_code++;

    if (m_position_in_code >= m_code.size()) return 0;
    return m_code[m_position_in_code++];
}
