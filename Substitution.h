#pragma once

#include "Common.h"
#include <map>
#include <iostream>
#include "ast/Expression.h"
#include "ast/Variable.h"
#include "ast/Function.h"

class Substitution {
private:
    std::map<std::string, std::shared_ptr<Expression>> mVarMap;
public:
    Substitution();

    void set(std::string var, std::shared_ptr<Expression> exp);
    std::shared_ptr<Expression> get(std::shared_ptr<Variable> var) const;

    ExpsT applyTo(const ExpsT& exps) const;
    std::shared_ptr<Expression> applyTo(std::shared_ptr<Expression> exp) const;
    std::shared_ptr<Expression> applyTo(std::shared_ptr<Variable> var) const;
    std::shared_ptr<Expression> applyTo(std::shared_ptr<Function> func) const;

    Substitution apply(const Substitution& other) const;

    static std::experimental::optional<Substitution> getMGU(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
    static std::experimental::optional<Substitution> getMGU(std::shared_ptr<Function> left, std::shared_ptr<Function> right);

    static Substitution renameVars(const std::set<std::string>& vars);

    Substitution filter(const std::set<std::string>& vars) const;

    friend inline std::ostream& operator <<(std::ostream& os, const Substitution& s) {
        int i = 0;
        int last = (int) s.mVarMap.size() - 1;

        if (last < 0) {
            std::cout << "True.";
        }
        else {
            for (auto mapEntry : s.mVarMap) {
                std::cout << mapEntry.first << " = " << *(mapEntry.second);
                if (i != last) {
                    std::cout << "," << std::endl;
                }

                i++;
            }
        }
        return os;
    }
};