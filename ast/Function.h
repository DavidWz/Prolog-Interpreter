#pragma once

#include "../Common.h"
#include <iostream>
#include <cctype>
#include "Expression.h"

/**
 * Represents a function.
 * A function is a function name followed by a list of expressions in parenthesis.
 * Function names have the form: [a-z][a-zA-Z0-9]*
 */
class Function : public Expression {
private:
    std::string mName;
    std::vector<std::shared_ptr<Expression>> mExpressions;
public:
    Function(const std::string& name, const std::vector<std::shared_ptr<Expression>> exps) :
        mName(name),
        mExpressions(exps)
    {
        assert(!mName.empty() && std::islower(mName[0]));
        for (int i=0; i<(int) mName.size(); i++) {
            assert(std::isalnum(mName[i]));
        }
    }

    std::string getName() const {
        return mName;
    }

    std::vector<std::shared_ptr<Expression>> getExpressions() const {
        return mExpressions;
    }

    void print(std::ostream& os) const {
        os << mName;
        if (!mExpressions.empty()) {
            os << "(" << mExpressions << ")";
        }
    }
};