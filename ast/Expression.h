#pragma once

#include <iostream>
#include "../Common.h"

/**
 * An expression is either a variable or a function.
 */
class Expression {
public:
    virtual bool contains(const std::string& var) const = 0;
    virtual std::set<std::string> getVariableNames() const = 0;

    bool operator==(const Expression& other) const {
        return this->equals(other);
    }

    bool operator!=(const Expression& other) const {
        return !this->equals(other);
    }

    friend inline std::ostream& operator <<(std::ostream& os, const Expression& exp) {
        exp.print(os);
        return os;
    }

    friend inline std::ostream& operator <<(std::ostream& os, const ExpsT& exps) {
        for (int i = 0; i < (int) exps.size(); i++) {
            exps[i]->print(os);
            if (i < (int) exps.size()-1) {
                os << ", ";
            }
        }
        return os;
    }

protected:
    virtual void print(std::ostream& os) const = 0;
    virtual bool equals(const Expression& other) const = 0;
};