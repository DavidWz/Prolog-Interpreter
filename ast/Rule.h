#pragma once

#include <iostream>
#include "../Common.h"
#include "Expression.h"

/**
 * A rule consists of a left-hand side expression and possible right-hand side expressions.
 */
class Rule {
private:
    std::shared_ptr<Expression> mLhs;
    std::vector<std::shared_ptr<Expression>> mRhs;
public:
    Rule(std::shared_ptr<Expression> lhs, const std::vector<std::shared_ptr<Expression>>& rhs) :
        mLhs(lhs),
        mRhs(rhs)
    {
    }

    Rule(std::shared_ptr<Expression> lhs) :
        mLhs(lhs),
        mRhs()
    {
    }

    std::shared_ptr<Expression> getLhs() const {
        return mLhs;
    }

    std::vector<std::shared_ptr<Expression>> getRhs() const {
        return mRhs;
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