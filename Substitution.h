#pragma once

#include "Common.h"
#include <unordered_map>
#include <iostream>
#include "ast/Expression.h"
#include "ast/Variable.h"
#include "ast/Function.h"

class Substitution {
private:
    std::unordered_map<std::shared_ptr<Variable>, std::shared_ptr<Expression>> mVarMap;
public:
    Substitution();

    void set(std::shared_ptr<Variable> var, std::shared_ptr<Expression> exp);
    std::shared_ptr<Expression> get(std::shared_ptr<Variable> var) const;

    std::shared_ptr<Expression> applyTo(std::shared_ptr<Expression> var);
    std::shared_ptr<Expression> applyTo(std::shared_ptr<Variable> var);
    std::shared_ptr<Expression> applyTo(std::shared_ptr<Function> func);

    void apply(const Substitution& other);

    static std::experimental::optional<Substitution> getMGU(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);

    friend inline std::ostream& operator <<(std::ostream& os, const Substitution& s) {
        bool first = true;
        for (auto mapEntry : s.mVarMap) {
            if (!first) {
                std::cout << ", ";
            }
            std::cout << *(mapEntry.first) << " = " << *(mapEntry.second);
            first = false;
        }
        return os;
    }
};