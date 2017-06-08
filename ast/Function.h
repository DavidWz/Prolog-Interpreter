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

    std::set<std::string> getVariableNames() const {
        std::set<std::string> vars;
        for (auto exp : mExpressions) {
            std::set<std::string> expVars = exp->getVariableNames();
            vars.insert(expVars.begin(), expVars.end());
        }
        return vars;
    }

    bool contains(const std::string& var) const {
        for (auto it : mExpressions) {
            if (it->contains(var)) {
                return true;
            }
        }
        return false;
    }

protected:
    void print(std::ostream& os) const {
        os << mName;
        if (!mExpressions.empty()) {
            os << "(" << mExpressions << ")";
        }
    }

    bool equals(const Expression& other) const {
        if (const Function* func = dynamic_cast<const Function*>(&other)) {
            if (mName != func->mName)
                return false;
            if (mExpressions.size() != func->mExpressions.size())
                return false;
            for (int i = 0; i < (int) mExpressions.size(); i++) {
                if (*(mExpressions[i]) != *(func->mExpressions[i]))
                    return false;
            }
            return true;
        }
        else {
            return false;
        }
    }
};