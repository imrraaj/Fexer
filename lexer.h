#ifndef LEXER_H
#define LEXER_H
#pragma once
#include <bits/stdc++.h>
using namespace std;

typedef enum
{
    C = 0,
    CPP,
    PYTHON,
    JAVASCRIPT,
} Language;

Language find_file_language(char *filename)
{
    string fn(filename);
    int ext = -1;
    for (int i = fn.size() - 1; i >= 0; i--)
    {
        if (fn[i] == '.')
        {
            ext = i + 1;
        }
    }
    string tmp = fn.substr(ext);
    Language lang;
    if (tmp == "c" || tmp == "h")
        lang = Language::C;
    else if (tmp == "cpp")
        lang = Language::CPP;
    else if (tmp == "py")
        lang = Language::PYTHON;
    else if (tmp == "js")
        lang = Language::JAVASCRIPT;
    else
    {
        cout << "ERROR: INVALID FILE FORMAT.\nOnly C (Source and Header Files), C++, Python, Javascript is supported\n";
        exit(1);
    }
    return lang;
}

typedef enum
{
    TOKEN_END = 0,
    TOKEN_INVALID,
    TOKEN_PREPROC,
    TOKEN_KEYWORD,
    TOKEN_SYMBOL,
    TOKEN_NUMBER,
    TOKEN_COMMENT,
    TOKEN_STRING,
    TOKEN_SPACE,

    TOKEN_NEWLINE,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_COMMA,

    TOKEN_OPEN_PARENTHESIS,
    TOKEN_CLOSE_PARENTHESIS,
    TOKEN_OPEN_CURLY,
    TOKEN_CLOSE_CURLY,
    TOKEN_OPEN_SQUARE,
    TOKEN_CLOSE_SQUARE,
    TOKEN_OPEN_ANGULAR,
    TOKEN_CLOSE_ANGULAR,

    TOKEN_EQUAL,
    TOKEN_TILDE,
    TOKEN_QUESTION,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_DIVIDE,
    TOKEN_MULTIPLY,
    TOKEN_XOR,
    TOKEN_OR,
    TOKEN_AND,
    TOKEN_NOT,
    TOKEN_PERCENT,
    TOKEN_DOLLAR,
} Token_Kind;

const char *token_kind_name(Token_Kind kind)
{
    switch (kind)
    {
    case TOKEN_END:
        return "TOKEN_END";
    case TOKEN_INVALID:
        return "TOKEN_INVALID";
    case TOKEN_PREPROC:
        return "TOKEN_PREPROC";
    case TOKEN_SYMBOL:
        return "TOKEN_SYMBOL";
    case TOKEN_OPEN_PARENTHESIS:
        return "OPEN parenthesis";
    case TOKEN_CLOSE_PARENTHESIS:
        return "CLOSE parenthesis";
    case TOKEN_OPEN_CURLY:
        return "OPEN curly";
    case TOKEN_CLOSE_CURLY:
        return "CLOSE curly";
    case TOKEN_SEMICOLON:
        return "TOKEN_SEMICOLON";
    case TOKEN_COLON:
        return "TOKEN_COLON";
    case TOKEN_KEYWORD:
        return "TOKEN_KEYWORD";
    case TOKEN_COMMENT:
        return "TOKEN_COMMENT";
    case TOKEN_STRING:
        return "TOKEN_STRING";
    case TOKEN_NUMBER:
        return "TOKEN_NUMBER";

    default:
        return "TOKEN KIND NAME UNREACHABLE";
    }
}

typedef struct
{
    Token_Kind kind;
    const char *text;
    size_t text_len;
} Token;

typedef struct
{
    const char *text;
    Token_Kind kind;
} Literal_tokens;
// unlike strings which can contain anything literal tokens are literally the c token itself we can assign without any checking

vector<Literal_tokens> literal_tokens = {
    {.text = "(", .kind = TOKEN_OPEN_PARENTHESIS},
    {.text = ")", .kind = TOKEN_CLOSE_PARENTHESIS},
    {.text = "{", .kind = TOKEN_OPEN_CURLY},
    {.text = "}", .kind = TOKEN_CLOSE_CURLY},
    {.text = "[", .kind = TOKEN_OPEN_SQUARE},
    {.text = "]", .kind = TOKEN_CLOSE_SQUARE},
    {.text = "<", .kind = TOKEN_OPEN_ANGULAR},
    {.text = ">", .kind = TOKEN_CLOSE_ANGULAR},
    {.text = ",", .kind = TOKEN_COMMA},

    {.text = ";", .kind = TOKEN_SEMICOLON},
    {.text = ":", .kind = TOKEN_COLON},
    {.text = "\n", .kind = TOKEN_NEWLINE},
    {.text = "=", .kind = TOKEN_EQUAL},
    {.text = "~", .kind = TOKEN_TILDE},
    {.text = "?", .kind = TOKEN_QUESTION},
    {.text = "+", .kind = TOKEN_PLUS},
    {.text = "-", .kind = TOKEN_MINUS},
    {.text = "/", .kind = TOKEN_DIVIDE},
    {.text = "*", .kind = TOKEN_MULTIPLY},
    {.text = "^", .kind = TOKEN_XOR},
    {.text = "|", .kind = TOKEN_OR},
    {.text = "&", .kind = TOKEN_AND},
    {.text = "!", .kind = TOKEN_NOT},
    {.text = "%", .kind = TOKEN_PERCENT},
    {.text = "$", .kind = TOKEN_DOLLAR},
    {.text = " ", .kind = TOKEN_SPACE},
};

vector<string> keywords;
void read_keywords(string fname)
{
    string line, word;
    keywords.clear();
    fstream file(fname, ios::in);
    if (file.is_open())
    {
        while (getline(file, line))
        {
            stringstream str(line);
            while (getline(str, word, ','))
                keywords.push_back(word);
        }
    }
}

class Lexer
{
    const char *content;
    size_t content_len;
    size_t cursor;
    size_t line;
    size_t bol; // begining of the line
    Language lang;

    void init_keyword_vec()
    {
        switch (this->lang)
        {
        case C:
        case CPP:
            read_keywords("keywords/c_keywords.csv");
            break;
        case JAVASCRIPT:
            read_keywords("keywords/js_keywords.csv");
            break;
        case PYTHON:
            read_keywords("keywords/py_keywords.csv");
            break;
        }
    }
    bool is_symbol_start(char x)
    {
        return isalpha(x) || x == '_';
    }
    bool is_symbol(char x)
    {
        return isalnum(x) || x == '_';
    }
    bool is_digit(char x)
    {
        // for hexadecimal and scientific notation
        return isdigit(x) || x == 'x' || x == 'X' || x == 'e' || x == 'E';
    }

    void chop_char(size_t len = 1)
    {
        for (size_t i = 0; i < len; i++)
        {
            assert(this->cursor < this->content_len);
            char x = this->content[this->cursor];
            this->cursor++;
            if (x == '\n')
            {
                this->line++;
                this->bol = this->cursor;
            }
        }
    }

    void trim_left()
    {
        while ((this->cursor < this->content_len) && isspace(this->content[this->cursor]))
        {
            chop_char(); // unused
        }
    }

    bool starts_with(const char *prefix)
    {
        size_t prefix_len = strlen(prefix);
        if (prefix_len == 0)
        {
            return true;
        }
        if (this->cursor + prefix_len - 1 >= this->content_len)
        {
            return false;
        }

        for (size_t i = 0; i < prefix_len; i++)
        {
            if (prefix[i] != this->content[this->cursor + i])
            {
                return false;
            }
        }
        return true;
    }

public:
    Lexer(const char *content, size_t content_len, Language l)
    {
        this->cursor = 0;
        this->content = content;
        this->content_len = content_len;
        this->lang = l;

        init_keyword_vec();
    }
    void setContent(const char *content, size_t content_len)
    {
        this->cursor = 0;
        this->content = content;
        this->content_len = content_len;
    }
    Token next()
    {
        // trim_left();
        Token token = {
            .kind = TOKEN_END,
            .text = &this->content[this->cursor],
            .text_len = 0,
        };

        if (this->cursor >= this->content_len)
        {
            return token;
        }
        if (this->content[this->cursor] == ' ')
        {
            token.kind = TOKEN_SPACE;
            chop_char();
            token.text_len = &this->content[this->cursor] - token.text + 1;
            return token;
        }

        if (this->content[this->cursor] == '"')
        {
            token.kind = TOKEN_STRING;
            chop_char();
            while ((this->cursor < this->content_len) && this->content[this->cursor] != '"')
            {
                chop_char();
            }
            token.text_len = &this->content[this->cursor] - token.text + 1;
            chop_char();
            return token;
        }

        if (this->content[this->cursor] == '#')
        {

            // preproc should handle the new line in between
            token.kind = TOKEN_PREPROC;
            while ((this->cursor < this->content_len) && this->content[this->cursor] != '\n')
            {
                chop_char();
            }
            if (this->cursor < this->content_len)
            {
                // it has a new line
                chop_char();
            }
            token.text_len = &this->content[this->cursor] - token.text;
            return token;
        }

        if (starts_with("//"))
        {
            token.kind = TOKEN_COMMENT;
            while ((this->cursor < this->content_len) && this->content[this->cursor] != '\n')
            {
                chop_char();
            }
            if (this->cursor < this->content_len)
            {
                // it has a new line
                chop_char();
            }
            token.text_len = &this->content[this->cursor] - token.text;
            return token;
        }

        for (int i = 0; i < (int)literal_tokens.size(); i++)
        {
            if (starts_with(literal_tokens[i].text))
            {
                size_t text_len = strlen(literal_tokens[i].text);
                token.text_len = text_len;
                token.kind = literal_tokens[i].kind;
                chop_char(text_len);
                return token;
            }
        }

        if (is_symbol_start(this->content[this->cursor]))
        {
            token.kind = TOKEN_SYMBOL;
            while (this->cursor < this->content_len && is_symbol(this->content[this->cursor]))
            {
                chop_char();
                token.text_len++;
            }

            for (int i = 0; i < (int)keywords.size(); i++)
            {
                size_t keyword_len = keywords[i].size();
                if (keyword_len == token.text_len && (memcmp(keywords[i].c_str(), token.text, keyword_len) == 0))
                {
                    token.kind = TOKEN_KEYWORD;
                    break;
                }
            }

            return token;
        }

        if (is_digit(this->content[this->cursor]))
        {
            token.kind = TOKEN_NUMBER;
            while (this->cursor < this->content_len && is_digit(this->content[this->cursor]))
            {
                chop_char();
                token.text_len++;
            }
            return token;
        }

        chop_char();
        token.kind = TOKEN_INVALID;
        token.text_len = 1;
        return token;
    }
};
#endif // LEXER_H
