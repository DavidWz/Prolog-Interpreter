#pragma once

#include "../Common.h"
#include <iostream>
#include <cctype>
#include "Expression.h"
#include "VariablePool.h"

/**
 * Represents a variable.
 * Variables have the form: [A-Z][a-zA-Z0-9]*
 */
class Variable : public Expression {
private:
    std::string mName;

public:
    Variable(const std::string& name) :
        mName(name)
    {
        assert(!mName.empty() && std::isupper(mName[0]));
        for (int i=0; i<(int)mName.size(); i++) {
            assert(std::isalnum(mName[i]));
        }
        VariablePool::add(mName);
    }

    std::string getName() const {
        return mName;
    }

    std::set<std::string> getVariableNames() const {
        std::set<std::string> vars;
        vars.insert(mName);
        return vars;
    }

    bool contains(const std::string& var) const {
        return (mName == var);
    }

protected:
    void print(std::ostream& os) const {
        os << mName;
    }

    bool equals(const Expression& other) const {
        if (const Variable* var = dynamic_cast<const Variable*>(&other)) {
            return (mName == var->mName);
        }
        return false;
    }
};