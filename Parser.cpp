#include "Parser.h"

#include <iostream>
#include <cctype>

Parser::Parser(std::ifstream& stream) :
    mContent(std::istreambuf_iterator<char>(stream), {}),
    mPos(0),
    mIsSuccessful(true)
{
}

void Parser::skipIgnoredChars() {
    while (mPos < (int) mContent.size() && isspace(mContent[mPos])) {
        mPos++;
    }
}

bool Parser::consume(char c) {
    if (hasNext()) {
        if (peek() == c) {
            consume();
            return true;
        }
        else {
            std::ostringstream oss;
            oss << "Expected '"<< c <<"' but got '"<< peek() <<"'.";
            fail(oss.str());
            return false;
        }
    }
    else {
        std::ostringstream oss;
        oss << "Expected '"<< c <<"' but reached end of input.";
        fail(oss.str());
        return false;
    }
}

char Parser::consume() {
    char c = mContent[mPos];
    mPos++;
    return c;
}

char Parser::peek() const {
    return mContent[mPos];
}

bool Parser::isSuccessful() const {
    return mIsSuccessful;
}

bool Parser::hasNext() const {
    return mPos < (int) mContent.size();
}

void Parser::fail(std::string msg) {
    if (mIsSuccessful) {
        std::cerr << msg << std::endl;
        std::cerr << "\taround " << mContent.substr(mPos, std::min(mPos, (int)mContent.size() - mPos)) << std::endl;

        mIsSuccessful = false;
        mPos = mContent.size(); 
    }
}

std::shared_ptr<Program> Parser::parse() {
    skipIgnoredChars();

    std::vector<std::shared_ptr<Rule>> rules;

    while (hasNext()) {
        std::shared_ptr<Rule> rule = parseRule();
        rules.push_back(rule);
    }

    return std::make_shared<Program>(rules);
}

std::shared_ptr<Rule> Parser::parseRule() {
    skipIgnoredChars();

    std::shared_ptr<Expression> lhs = parseExpression();
    std::vector<std::shared_ptr<Expression>> rhs;

    skipIgnoredChars();

    if (peek() == ':') {
        if (consume(':') && consume('-')) {
            rhs = parseExpressions();
        }
    }

    skipIgnoredChars();
    consume('.');

    return std::make_shared<Rule>(lhs, rhs);
}

std::shared_ptr<Expression> Parser::parseExpression() {
    skipIgnoredChars();

    std::shared_ptr<Expression> exp;

    if (isupper(peek())) {
        exp = parseVariable();
    }
    else if (islower(peek())) {
        exp = parseFunction();
    }
    else {
        std::ostringstream oss;
        oss << "Expected variable or function declaration, but got '" << peek() << "'.";
        fail(oss.str());
    }

    return exp;
}

std::vector<std::shared_ptr<Expression>> Parser::parseExpressions() {
    skipIgnoredChars();

    std::vector<std::shared_ptr<Expression>> exps;

    exps.push_back(parseExpression());

    while (peek() == ',') {
        consume(',');
        exps.push_back(parseExpression());
    }

    return exps;
}

std::shared_ptr<Function> Parser::parseFunction() {
    skipIgnoredChars();

    if (!islower(peek())) {
        std::ostringstream oss;
        oss << "Expected lower case character for function declaration, but got '" << peek() << "'";
        fail(oss.str());
        return std::shared_ptr<Function>();
    }

    std::string name = "";
    std::vector<std::shared_ptr<Expression>> exps;

    name += consume();
    while (std::isalnum(peek())) {
        name += consume();
    }

    if (peek() == '(') {
        consume('(');
        exps = parseExpressions();
        consume(')');
    }

    return std::make_shared<Function>(name, exps);
}

std::shared_ptr<Variable> Parser::parseVariable() {
    skipIgnoredChars();

    std::shared_ptr<Variable> var;

    if (!isupper(peek())) {
        std::ostringstream oss;
        oss << "Expected upper case character for variable declaration, but got '" << peek() << "'.";
        fail(oss.str());
    }
    else {
        std::string name = "";

        name += consume();
        while (std::isalnum(peek())) {
            name += consume();
        }

        var = std::make_shared<Variable>(name);
    }

    return var;
}