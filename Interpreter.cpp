#include "Interpreter.h"
#include "Substitution.h"
#include "ast/Expression.h"
#include <algorithm>

Interpreter::Interpreter(std::shared_ptr<Program> program) :
    mProgram(program)
{
}

void printExps(const ExpsT& exps) {
    for (auto exp : exps) {
        std::cout << *exp << ", ";
    }
}

std::set<std::string> getVariableNames(const ExpsT& exps) {
    std::set<std::string> vars;
    for (auto exp : exps) {
        std::set<std::string> expVars = exp->getVariableNames();
        vars.insert(expVars.begin(), expVars.end());
    }
    return vars;
}

std::experimental::optional<Substitution> Interpreter::evaluate(ExpsT query) {
    ExpsT negativeClause = query;
    Substitution answer;

#ifdef DEBUG
    std::cout << "Evaluating ";
    printExps(negativeClause);
    std::cout << std::endl;
#endif

    // try to resolve the query until we get the empty clause
    while (negativeClause.size() > 0) {
        std::shared_ptr<Expression> negativeLiteral = negativeClause[0];

#ifdef DEBUG
        std::cout << "Current negative clause: ";
        printExps(negativeClause);
        std::cout << std::endl;
        std::cout << "Current answer: " << answer << std::endl;
        std::cout << "Picked negative literal " << *negativeLiteral << std::endl;
#endif
    
        // try to resolve the negative clause with a definite program clause
        bool isUnifiable = false;
        for (int i = 0; i < (int) mProgram->getRules().size(); i++) {
            std::shared_ptr<Rule> definiteClause = mProgram->getRules()[i];
            std::shared_ptr<Expression> positiveLiteral = definiteClause->getLhs();

            Substitution renaming = Substitution::renameVars(definiteClause->getVariableNames());
            std::shared_ptr<Expression> renamedPositiveLiteral = renaming.applyTo(positiveLiteral);

#ifdef DEBUG
            std::cout << "Trying to unify " << *negativeLiteral << " with " << *positiveLiteral << std::endl;
            std::cout << "Renamed " << *positiveLiteral << " to " << *renamedPositiveLiteral << std::endl;
#endif

            // if the negative and positive clause are unifiable
            std::experimental::optional<Substitution> mgu = Substitution::getMGU(negativeLiteral, renamedPositiveLiteral);
            if (mgu) {
                isUnifiable = true;
#ifdef DEBUG
                std::cout << "MGU found: " << *mgu << std::endl;
#endif

                // resolve the clauses
                auto it = std::find(negativeClause.begin(), negativeClause.end(), negativeLiteral);
                negativeClause.erase(it);
                negativeClause = unify(negativeClause, renaming.applyTo(definiteClause->getRhs()), *mgu);

                // update answer
                answer.apply(*mgu);

                break;
            }
            else {
#ifdef DEBUG
                std::cout << "No MGU found." << std::endl;
#endif
                isUnifiable = false;
            }
        }

        if (!isUnifiable) {
#ifdef DEBUG
            std::cout << "Could not unify with any clause." << std::endl;
#endif
            // return empty indicating not resolvable
            return std::experimental::optional<Substitution>();
        }
    }

    // the answer substitution only needs to contain variables from the query
    std::set<std::string> origVars = getVariableNames(query);
    return answer.filter(origVars);
}

bool contains(const ExpsT& exps, std::shared_ptr<Expression> exp) {
    for (std::shared_ptr<Expression> it : exps) {
        if (*it == *exp) {
            return true;
        }
    }
    return false;
}

ExpsT Interpreter::unify(const ExpsT& left, const ExpsT& right, const Substitution& mgu) {
    ExpsT result;

    for (int i = 0; i < (int) left.size(); i++) {
        std::shared_ptr<Expression> exp = mgu.applyTo(left[i]);
        if (!contains(result, exp)) {
            result.push_back(exp);
        }
    }
    for (int i = 0; i < (int) right.size(); i++) {
        std::shared_ptr<Expression> exp = mgu.applyTo(right[i]);
        if (!contains(result, exp)) {
            result.push_back(exp);
        }
    }

    return result;
}