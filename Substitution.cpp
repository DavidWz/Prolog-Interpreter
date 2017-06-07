#include "Substitution.h"

Substitution::Substitution() 
{
}

void Substitution::set(std::shared_ptr<Variable> var, std::shared_ptr<Expression> exp) {
    mVarMap[var] = exp;
}

std::shared_ptr<Expression> Substitution::get(std::shared_ptr<Variable> var) const {
    auto it = mVarMap.find(var);
    if (it != mVarMap.end()) {
        return it->second;
    }
    else {
        return var;
    }
}

std::shared_ptr<Expression> Substitution::applyTo(std::shared_ptr<Expression> exp) {
    if (std::shared_ptr<Variable> var = std::dynamic_pointer_cast<Variable>(exp)) {
        return applyTo(var);
    }
    else if (std::shared_ptr<Function> func = std::dynamic_pointer_cast<Function>(exp)) {
        return applyTo(func);
    }
    else {
        // Error
        assert(false);
        return std::shared_ptr<Expression>();
    }
}

std::shared_ptr<Expression> Substitution::applyTo(std::shared_ptr<Variable> var) {
    return get(var);
}

std::shared_ptr<Expression> Substitution::applyTo(std::shared_ptr<Function> func) {
    std::vector<std::shared_ptr<Expression>> exps;
    for (int i = 0; i < (int) func->getExpressions().size(); i++) {
        exps.push_back(applyTo(func->getExpressions()[i]));
    }
    return std::make_shared<Function>(func->getName(), exps);
}

void Substitution::apply(const Substitution& other) {
    // TODO
}

std::experimental::optional<Substitution> Substitution::getMGU(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
    // TODO
    return std::experimental::optional<Substitution>();
}