#include "lexer.h"
#include <string.h>

#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"

void lexer_init(Lexer* lexer) {
    lexer->position_in_code = 0;
    lexer->code = NULL;
}

void lexer_fill(Lexer* lexer, char* code) {
    for (size_t i = 0; i < arrlen(code); i++) {
        arrpush(lexer->code, code[i]);
    }
}

bool lexer_is_valid_instruction(char instruction) {
    char* valid_insts = "><+-.,[]";
    if (strchr(valid_insts, instruction) != NULL)
        return true;
    else
        return false;
}

char lexer_next(Lexer* lexer) {
    while (lexer->position_in_code < arrlen(lexer->code) && !lexer_is_valid_instruction(lexer->code[lexer->position_in_code])) {
        lexer->position_in_code++;
    }

    if (lexer->position_in_code >= arrlen(lexer->code)) return 0;
    return lexer->code[lexer->position_in_code++];
}

void lexer_free(Lexer* lexer) {
    arrfree(lexer->code);
}
