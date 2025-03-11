#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <vector>
#include <cstdint>

class Lexer {
    public:
        Lexer();

        void fill(const std::string& code);
        char next();
    private:
        bool is_valid_instruction(char instruction);

        std::vector<char> m_code;
        uint64_t m_position_in_code;
};

#endif
