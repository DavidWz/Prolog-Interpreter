#include "Parser.h"

#include <iostream>
#include <cctype>

Parser::Parser(std::ifstream& stream) :
    mContent(std::istreambuf_iterator<char>(stream), {}),
    mPos(0),
    mIsSuccessful(true)
{
}

Parser::Parser(const std::string& input) :
    mContent(input),
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
        mIsSuccessful = false;
        mPos = mContent.size(); 
    }
}

std::shared_ptr<Program> Parser::parseProgram() {
    skipIgnoredChars();

    std::vector<std::shared_ptr<Rule>> rules;

    while (hasNext()) {
        std::shared_ptr<Rule> rule = parseRule();
        rules.push_back(rule);
        skipIgnoredChars();
    }

    return std::make_shared<Program>(rules);
}

std::shared_ptr<Rule> Parser::parseRule() {
    skipIgnoredChars();

    std::shared_ptr<Function> lhs = parseFunction();
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
    else if (peek() == '.' || peek() == '[') {
        exp = parseList();
    }
    else if (peek() == '-' || isdigit(peek())) {
        exp = parseInteger();
    }
    else {
        std::ostringstream oss;
        oss << "Expected expression, but got '" << peek() << "'.";
        fail(oss.str());
    }

    return exp;
}

ExpsT Parser::parseExpressions() {
    skipIgnoredChars();

    ExpsT exps;

    exps.push_back(parseExpression());

    while (peek() == ',') {
        consume(',');
        exps.push_back(parseExpression());
    }

    return exps;
}

ExpsT Parser::parseQuery() {
    skipIgnoredChars();

    ExpsT exps = parseExpressions();
    skipIgnoredChars();
    consume('.');

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

std::shared_ptr<Function> Parser::parseList() {
    skipIgnoredChars();

    std::shared_ptr<Function> func;
    ExpsT exps;

    if (peek() == '.') {
        // .(Exp, List)
        consume('.');
        consume('(');

        exps.push_back(parseExpression());
        skipIgnoredChars();

        consume(',');
        skipIgnoredChars();

        if (isupper(peek())) {
            exps.push_back(parseVariable());
        }
        else if (peek() == '.' || peek() == '[') {
            exps.push_back(parseList());
        }
        else {
            std::ostringstream oss;
            oss << "Expected variable or list declaration, but got '" << peek() << "'.";
            fail(oss.str());
        }

        consume(')');

        func = std::make_shared<Function>(".", exps);
    }
    else {
        consume('[');

        if (peek() == ']') {
            consume(']');
            // []
            func = std::make_shared<Function>("[]");
        }
        else {
            //   [exp1, exp2, ..., expn | tail]
            ExpsT listMembers = parseExpressions();
            skipIgnoredChars();
            
            std::shared_ptr<Expression> tail;
            if (peek() == '|') {
                consume('|');
                skipIgnoredChars();
                
                if (isupper(peek())) {
                    tail = parseVariable();
                }
                else if (peek() == '.' || peek() == '[') {
                    tail = parseList();
                }
                else {
                    std::ostringstream oss;
                    oss << "Expected variable or list declaration, but got '" << peek() << "'.";
                    fail(oss.str());
                }
            }
            else {
                tail = std::make_shared<Function>("[]");
            }
            consume(']');

            //=> .(exp1, .(exp2, .(... .(expn, tail) ...)))
            func = std::make_shared<Function>(".", listMembers[(int)listMembers.size()-1], tail);
            for (int i = (int) listMembers.size() - 2; i >= 0; i--) {
                func = std::make_shared<Function>(".", listMembers[i], func);
            }
        }
    }

    return func;
}

std::shared_ptr<Function> Parser::parseInteger() {
    skipIgnoredChars();

    std::shared_ptr<Function> func;

    std::string numberStr = "";

    if (peek() == '-') {
        numberStr += consume();
    }
    
    if (isdigit(peek())) {
        do {
            numberStr += consume();
        } while(isdigit(peek()));

        constexpr int numBits = 8;
        int8_t number = std::stoi(numberStr);

        ExpsT bits;
        for (int i = 0; i < numBits; i++) {
            int b = number & (1 << (numBits-i-1));
            std::shared_ptr<Function> bit;
            if (b == 0) {
                bit = std::make_shared<Function>("b0");
            }
            else {
                bit = std::make_shared<Function>("b1");
            }
            bits.push_back(bit);
        }

        func = std::make_shared<Function>("i8", bits);
    }
    else {
        std::ostringstream oss;
        oss << "Expected digit but got '" << peek() << "'.";
        fail(oss.str());
    }

    return func;
}