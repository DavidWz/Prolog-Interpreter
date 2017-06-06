#pragma once

#include <iostream>
#include "../Common.h"

/**
 * An expression is either a variable or a function.
 */
class Expression {
public:
    virtual void print(std::ostream& os) const {
        os << "{NOT IMPLEMENTED}";
    }

    friend inline std::ostream& operator <<(std::ostream& os, const Expression& exp) {
        exp.print(os);
        return os;
    }
};