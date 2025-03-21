#ifndef LEXER_H
#define LEXER_H

#include <stdint.h>

typedef struct {
    uint64_t position_in_code;
    char* code;
} Lexer;

void lexer_init(Lexer* lexer);
void lexer_fill(Lexer* lexer, char* code);
bool lexer_is_valid_instruction(char instruction);
char lexer_next(Lexer* lexer);
void lexer_free(Lexer* lexer);

#endif
