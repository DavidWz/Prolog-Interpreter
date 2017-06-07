#pragma once

#include "../Common.h"
#include <iostream>
#include <cctype>
#include "Expression.h"

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
    }

    std::string getName() const {
        return mName;
    }

    void print(std::ostream& os) const {
        os << mName;
    }

    bool operator==(const Variable& other) const {
        return (mName == other.mName);
    }
};

namespace std {
    template <>
    struct hash<Variable> {
        std::size_t operator()(const Variable& var) const {
            using std::size_t;
            using std::hash;
            using std::string;
            return hash<string>()(var.getName());
        }
    };
}