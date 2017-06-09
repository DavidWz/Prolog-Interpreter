#include "Substitution.h"

Substitution::Substitution() 
{
}

void Substitution::set(std::string var, std::shared_ptr<Expression> exp) {
    mVarMap[var] = exp;

    // recursive update
    for (auto mapEntry : mVarMap) {
        if (mapEntry.second->contains(var)) {
            mVarMap[mapEntry.first] = applyTo(mapEntry.second);
        }
    }
}

std::shared_ptr<Expression> Substitution::get(std::shared_ptr<Variable> var) const {
    auto it = mVarMap.find(var->getName());
    if (it != mVarMap.end()) {
        return it->second;
    }
    else {
        return var;
    }
}

ExpsT Substitution::applyTo(const ExpsT& exps) const {
    ExpsT result;
    for (auto exp : exps) {
        result.push_back(applyTo(exp));
    }
    return result;
}

std::shared_ptr<Expression> Substitution::applyTo(std::shared_ptr<Expression> exp) const {
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

std::shared_ptr<Expression> Substitution::applyTo(std::shared_ptr<Variable> var) const {
    return get(var);
}

std::shared_ptr<Expression> Substitution::applyTo(std::shared_ptr<Function> func) const {
    std::vector<std::shared_ptr<Expression>> exps;
    for (int i = 0; i < (int) func->getExpressions().size(); i++) {
        exps.push_back(applyTo(func->getExpressions()[i]));
    }
    return std::make_shared<Function>(func->getName(), exps);
}

Substitution Substitution::apply(const Substitution& other) const {
    Substitution result;
    for (auto mapEntry : mVarMap) {
        result.set(mapEntry.first, mapEntry.second);
    }
    for (auto mapEntry : other.mVarMap) {
        result.set(mapEntry.first, mapEntry.second);
    }
    return result;
}

std::experimental::optional<Substitution> Substitution::getMGU(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
#ifdef DEBUG
    std::cout << "\tmgu(" << *left << ", " << *right  << "):" << std::endl;
#endif
    if (std::shared_ptr<Variable> leftVar = std::dynamic_pointer_cast<Variable>(left)) {
        if (right->contains(leftVar->getName())) {
            // occur failure
#ifdef DEBUG
            std::cout << "\tOccur Failure." << std::endl;
#endif
            return std::experimental::optional<Substitution>();
        }
        else {
            // var | exp
            Substitution s;
            s.set(leftVar->getName(), right);
#ifdef DEBUG
            std::cout << "\tRenaming " << *leftVar << " / " << *right << std::endl;
#endif
            return s;
        }
    }
    else if (std::shared_ptr<Variable> rightVar = std::dynamic_pointer_cast<Variable>(right)) {
        if (left->contains(rightVar->getName())) {
            // occur failure
#ifdef DEBUG
            std::cout << "\tOccur Failure." << std::endl;
#endif
            return std::experimental::optional<Substitution>();
        }
        else {
            // func | var
            Substitution s;
            s.set(rightVar->getName(), left);
#ifdef DEBUG
            std::cout << "\tRenaming " << *rightVar << " / " << *left << std::endl;
#endif
            return s;
        }
    }
    else {
        // func | func
        std::shared_ptr<Function> leftFunc = std::dynamic_pointer_cast<Function>(left);
        std::shared_ptr<Function> rightFunc = std::dynamic_pointer_cast<Function>(right);
        return getMGU(leftFunc, rightFunc);
    }
}

std::experimental::optional<Substitution> Substitution::getMGU(std::shared_ptr<Function> left, std::shared_ptr<Function> right) {
    if (left->getName() == right->getName() && left->getExpressions().size() == right->getExpressions().size()) {
        Substitution s;

        // unify as long as they are not equal yet
        while (*left != *right) {
            // find expression that differs
            int ineqPos = 0;
            while (*(left->getExpressions()[ineqPos]) == *(right->getExpressions()[ineqPos])) {
                ineqPos++;
            }

            // unify that expression
            std::experimental::optional<Substitution> tmp = getMGU(left->getExpressions()[ineqPos], right->getExpressions()[ineqPos]);
            if (tmp) {
                // apply the mgu to our expressions
                left = std::dynamic_pointer_cast<Function>(tmp->applyTo(left));
                right = std::dynamic_pointer_cast<Function>(tmp->applyTo(right));

                // add the mgu to our total substitution
#ifdef DEBUG
                std::cout << "\tApplying " << *tmp << " to " << s << std::endl;
#endif
                s = s.apply(*tmp);
#ifdef DEBUG
                std::cout << "\tResulting mgu: " << s << std::endl;
#endif
            }
            else {
                // failure
                return std::experimental::optional<Substitution>();
            }
        }

        return s;
    }
    else {
        // clash failure
#ifdef DEBUG
        std::cout << "\tClash Failure." << std::endl;
#endif
        return std::experimental::optional<Substitution>();
    }
}

Substitution Substitution::renameVars(const std::set<std::string>& vars) {
    Substitution s;
    for (std::string v : vars) {
        s.set(v, std::make_shared<Variable>(VariablePool::getFreshName()));
    }
    return s;
}

Substitution Substitution::filter(const std::set<std::string>& vars) const {
    Substitution s;
    for (auto mapEntry : mVarMap) {
        if (vars.count(mapEntry.first) > 0) {
            s.mVarMap[mapEntry.first] = mapEntry.second;
        }
    }
    return s;
}