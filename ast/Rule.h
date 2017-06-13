#pragma once

#include <iostream>
#include "../Common.h"
#include "Expression.h"
#include "Function.h"

/**
 * A rule consists of a left-hand side expression and possible right-hand side expressions.
 */
class Rule {
private:
    std::shared_ptr<Function> mLhs;
    std::vector<std::shared_ptr<Expression>> mRhs;
public:
    Rule(std::shared_ptr<Function> lhs, const std::vector<std::shared_ptr<Expression>>& rhs) :
        mLhs(lhs),
        mRhs(rhs)
    {
    }

    Rule(std::shared_ptr<Function> lhs) :
        mLhs(lhs),
        mRhs()
    {
    }

    std::shared_ptr<Function> getLhs() const {
        return mLhs;
    }

    std::vector<std::shared_ptr<Expression>> getRhs() const {
        return mRhs;
    }

    std::set<std::string> getVariableNames() const {
        std::set<std::string> vars = mLhs->getVariableNames();
        for (auto exp : mRhs) {
            std::set<std::string> expVars = exp->getVariableNames();
            vars.insert(expVars.begin(), expVars.end());
        }
        return vars;
    }

    friend inline std::ostream& operator <<(std::ostream& os, const Rule& rule) {
        os << *(rule.mLhs);
        if (!rule.mRhs.empty()) {
            os << " :- " << rule.mRhs;
        }
        os << ".";
        return os;
    }
};