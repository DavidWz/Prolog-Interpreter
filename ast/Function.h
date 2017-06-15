#pragma once

#include "../Common.h"
#include <iostream>
#include <cctype>
#include "Expression.h"
#include "Variable.h"

/**
 * Represents a function.
 * A function is a function name followed by a list of expressions in parenthesis.
 * Function names have the form: [a-z][a-zA-Z0-9]* or "[]" or "." in the case of lists.
 */
class Function : public Expression {
private:
    std::string mName;
    std::vector<std::shared_ptr<Expression>> mExpressions;

public:
    Function(const std::string& name) :
        mName(name),
        mExpressions()
    {
    }

    Function(const std::string& name, const std::vector<std::shared_ptr<Expression>> exps) :
        mName(name),
        mExpressions(exps)
    {
    }

    Function(const std::string& name, std::shared_ptr<Expression> exp1, std::shared_ptr<Expression> exp2) :
        mName(name),
        mExpressions()
    {
        mExpressions.push_back(exp1);
        mExpressions.push_back(exp2);
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
        // pretty printing for lists
        if (mName == ".") {
            os << "[" << *(mExpressions[0]);
            std::shared_ptr<Expression> tail = mExpressions[1];
            bool hasNext = true;
            while (hasNext) {
                if (std::shared_ptr<Variable> var = std::dynamic_pointer_cast<Variable>(tail)) {
                    os << " | " << *var << "]";
                    hasNext = false;
                }
                else if (std::shared_ptr<Function> func = std::dynamic_pointer_cast<Function>(tail)) {
                    if (func->mName == "[]") {
                        os << "]";
                        hasNext = false;
                    }
                    else {
                        os << ", " << *(func->mExpressions[0]);
                        tail = func->mExpressions[1];
                    }
                }
            }
        }
        else if (mName == "i8") {
            int8_t n = 0;
            int k = 1;
            bool hasVar = false;
            for (int i = (int)mExpressions.size()-1; i >= 0; i--) {
                if (mExpressions[i]->getName() == "b1") {
                    n += k;
                }
                else if(mExpressions[i]->getName() != "b0") {
                    hasVar = true;
                    break;
                }
                k <<= 1;
            }

            if (hasVar) {
                os << mName << "(" << mExpressions << ")";
            }
            else {
                os << (int) n;
            }
        }
        else {
            os << mName;
            if (!mExpressions.empty()) {
                os << "(" << mExpressions << ")";
            }
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