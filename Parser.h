#pragma once

#include <fstream>
#include <sstream>
#include "Common.h"
#include "ast/Program.h"
#include "ast/Rule.h"
#include "ast/Expression.h"
#include "ast/Function.h"
#include "ast/Variable.h"

class Parser {
private:
    // the program as a string
    std::string mContent;

    // the current position of the parser
    int mPos;

    // stores whether the parsing was successful
    bool mIsSuccessful;

public:
    Parser(std::ifstream& stream);
    Parser(const std::string& input);

    std::shared_ptr<Program> parseProgram();
    ExpsT parseQuery();

    bool isSuccessful() const;

private:
    std::shared_ptr<Rule> parseRule();
    std::shared_ptr<Expression> parseExpression();
    ExpsT parseExpressions();
    std::shared_ptr<Function> parseFunction();
    std::shared_ptr<Variable> parseVariable();
    std::shared_ptr<Function> parseList();

    // skips whitespaces etc
    void skipIgnoredChars();

    // expects the character c and consumes it if found, fails if not found
    bool consume(char c);

    // consumes the next character and returns it
    char consume();

    // returns the next character without consuming it
    char peek() const;

    // true if there is a next character
    bool hasNext() const;

    // aborts parsing with the given error message
    void fail(std::string msg);
};