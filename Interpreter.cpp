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

std::experimental::optional<Substitution> Interpreter::evaluate(const ExpsT& query) const {
    std::experimental::optional<Substitution> answer = evaluate(query, Substitution());
    
    if (answer) {
        // the answer substitution only needs to contain variables from the query
        std::set<std::string> origVars = getVariableNames(query);
        return answer->filter(origVars);
    }
    else {
        return answer;
    }
}

std::experimental::optional<Substitution> Interpreter::evaluate(const ExpsT& query, const Substitution& currentAnswer) const {
#ifdef DEBUG
    std::cout << "Evaluating ";
    printExps(query);
    std::cout << std::endl;
#endif

    if (query.size() == 0) {
        // Empty clause resolved, Done.
        return std::experimental::optional<Substitution>(currentAnswer);
    }

    // we simply pick the first literal as the next one to be resolved
    std::shared_ptr<Expression> negativeLiteral = query[0];

#ifdef DEBUG
    std::cout << "Current negative clause: ";
    printExps(query);
    std::cout << std::endl;
    std::cout << "Current answer: " << currentAnswer << std::endl;
    std::cout << "Picked negative literal " << *negativeLiteral << std::endl;
#endif

    std::experimental::optional<Substitution> finalAnswer;

    // try to resolve the negative clause with a definite program clause
    for (int i = 0; i < (int) mProgram->getRules().size(); i++) {
        std::shared_ptr<Rule> definiteClause = mProgram->getRules()[i];
        std::shared_ptr<Expression> positiveLiteral = definiteClause->getLhs();

        Substitution renaming = Substitution::renameVars(definiteClause->getVariableNames());
        std::shared_ptr<Expression> renamedPositiveLiteral = renaming.applyTo(positiveLiteral);

#ifdef DEBUG
        std::cout << "Trying to unify " << *negativeLiteral << " with " << *renamedPositiveLiteral << std::endl;
#endif
        // if the negative and positive clause are unifiable
        std::experimental::optional<Substitution> mgu = Substitution::getMGU(negativeLiteral, renamedPositiveLiteral);
        if (mgu) {
#ifdef DEBUG
            std::cout << "MGU found: " << *mgu << std::endl;
#endif
            // resolve the clauses
            ExpsT nextQuery = unify(query, negativeLiteral, renaming.applyTo(definiteClause->getRhs()), *mgu);

            // update currentAnswer
            Substitution nextAnswer = currentAnswer.apply(*mgu);

            // put the next configuration on the backtracking stack (i.e. the program stack)
            std::experimental::optional<Substitution> tmpFinalAnswer = evaluate(nextQuery, nextAnswer);
            if (tmpFinalAnswer) {
                // we found a final answer
                finalAnswer = tmpFinalAnswer;
                break;
            }
        }
        else {
#ifdef DEBUG
            std::cout << "No MGU found." << std::endl;
#endif
        }
    }

    if (!finalAnswer) {
#ifdef DEBUG
        std::cout << "Could not unify with any clause." << std::endl;
#endif
    }
    
    return finalAnswer;
}

bool contains(const ExpsT& exps, std::shared_ptr<Expression> exp) {
    for (std::shared_ptr<Expression> it : exps) {
        if (*it == *exp) {
            return true;
        }
    }
    return false;
}

ExpsT Interpreter::unify(const ExpsT& left, std::shared_ptr<Expression> resolvedLiteral, const ExpsT& right, const Substitution& mgu) const {
    ExpsT result;

    for (int i = 0; i < (int) right.size(); i++) {
        std::shared_ptr<Expression> exp = mgu.applyTo(right[i]);
        if (!contains(result, exp)) {
            result.push_back(exp);
        }
    }
    for (int i = 0; i < (int) left.size(); i++) {
        if (left[i] != resolvedLiteral) {
            std::shared_ptr<Expression> exp = mgu.applyTo(left[i]);
            if (!contains(result, exp)) {
                result.push_back(exp);
            }
        }
    }

    return result;
}