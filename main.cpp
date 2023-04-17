#include <bits/stdc++.h>
#include "./lexer.h"
#include "./highlight.h"
using namespace std;

int main(int argc, char **argv)
{
    string program;
    if (argc < 2)
    {
        cerr << "ERROR: Filename is not provided\n";
        exit(1);
    }
    ifstream filestream(argv[1]);
    string s;
    s += (char)filestream.get();
    while (filestream.good())
    {
        s += filestream.get();
    }
    program = s;
    filestream.close();
    Language lang = find_file_language(argv[1]);
    Lexer lex = Lexer(program.c_str(), program.size(), lang);
    vector<Token> tokenised_program;
    Token tok;
    tok = lex.next();
    while (tok.kind != TOKEN_END)
    {
        tokenised_program.push_back(tok);
        tok = lex.next();
    }
    HightLight output = HightLight(tokenised_program);
    output.printToConsole();
    return 0;
}

/*

For C/C++ Multiline comment is not supported.




*/