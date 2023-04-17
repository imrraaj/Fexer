#ifndef HIGHTLIGHT_H
#define HIGHTLIGHT_H
#pragma once
#include "./lexer.h"
using namespace std;

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#define BBLK "\e[1;30m"
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define BYEL "\e[1;33m"
#define BBLU "\e[1;34m"
#define BMAG "\e[1;35m"
#define BCYN "\e[1;36m"
#define BWHT "\e[1;37m"

// Regular background
#define BLKB "\e[40m"
#define REDB "\e[41m"
#define GRNB "\e[42m"
#define YELB "\e[43m"
#define BLUB "\e[44m"
#define MAGB "\e[45m"
#define CYNB "\e[46m"
#define WHTB "\e[47m"

class HightLight
{
    vector<Token> prog;
    int cursor;

public:
    HightLight(vector<Token> &tokenised_prog)
    {
        prog = tokenised_prog;
        cursor = 0;
    }
    void printToken(Token &t)
    {
        if (t.kind == TOKEN_END)
        {
            return;
        }
        if (t.kind == TOKEN_SPACE)
        {
            printf(" ");
            return;
        }
        if (t.kind == TOKEN_NEWLINE)
        {
            printf("\n");
            return;
        }
        switch (t.kind)
        {
        case TOKEN_INVALID:
            cout << RED;
            break;
        case TOKEN_SYMBOL:
            cout << RESET;
            break;
        case TOKEN_KEYWORD:
            cout << BLU;
            break;
        case TOKEN_PREPROC:
            cout << WHT;
            break;
        case TOKEN_OPEN_PARENTHESIS:
        case TOKEN_CLOSE_PARENTHESIS:
        case TOKEN_OPEN_CURLY:
        case TOKEN_CLOSE_CURLY:
            cout << YEL;
            break;
        case TOKEN_SEMICOLON:
        case TOKEN_COLON:
            cout << WHT;
            break;
        case TOKEN_COMMENT:
            cout << MAG;
            break;
        case TOKEN_STRING:
            cout << GRN;
            break;
        case TOKEN_NUMBER:
            cout << WHT;
            break;

        case TOKEN_OPEN_SQUARE:
        case TOKEN_CLOSE_SQUARE:
        case TOKEN_OPEN_ANGULAR:
        case TOKEN_CLOSE_ANGULAR:
        case TOKEN_EQUAL:
        case TOKEN_TILDE:
        case TOKEN_QUESTION:
        case TOKEN_PLUS:
        case TOKEN_MINUS:
        case TOKEN_DIVIDE:
        case TOKEN_MULTIPLY:
        case TOKEN_XOR:
        case TOKEN_OR:
        case TOKEN_AND:
        case TOKEN_NOT:
        case TOKEN_PERCENT:
        case TOKEN_DOLLAR:
        case TOKEN_COMMA:
            cout << CYN;
            break;
        case TOKEN_NEWLINE:
        case TOKEN_SPACE:
        case TOKEN_END:
            break;
        }
        printf("%.*s", (int)t.text_len, t.text);
        // cout << RESET;
    }
    void printToConsole()
    {
        for (auto it : prog)
        {
            printToken(it);
        }
    }
};
#endif // HIGHTLIGHT_H