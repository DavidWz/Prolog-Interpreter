#pragma once

#include "Common.h"
#include "ast/Expression.h"

class Substitution {
private:

public:
    Substitution();
    Substitution apply(Substitution other);
    static std::experimental::optional<Substitution> getMGU(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);

    friend inline std::ostream& operator <<(std::ostream& os, const Substitution& s) {
        os << "{}";
        return os;
    }
};