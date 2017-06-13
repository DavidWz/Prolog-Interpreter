#include "Interpreter.h"
#include "Substitution.h"
#include "ast/Expression.h"
#include <algorithm>

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

Interpreter::Interpreter(std::shared_ptr<Program> program) :
    mProgram(program),
    mSearchStack(),
    mOrigVars()
{
}

void Interpreter::init(const ExpsT& query) {
    mSearchStack = std::stack<Configuration>();
    mSearchStack.push(Configuration(query));
    mOrigVars = getVariableNames(query);
}

bool Interpreter::hasNext() {
    return !mSearchStack.empty();
}

std::experimental::optional<Substitution> Interpreter::nextAnswer() {
    while (!mSearchStack.empty()) {
        Configuration config = mSearchStack.top();
        mSearchStack.pop();

        // Empty clause resolved, Done.
        if (config.query.size() == 0) {
            return std::experimental::optional<Substitution>(config.answer.filter(mOrigVars));
        }

        // we simply pick the first literal as the next one to be resolved
        std::shared_ptr<Expression> negativeLiteral = config.query[0];

        // try to resolve the negative clause with a definite program clause
        for (int i = mProgram->getRules().size() - 1; i >= 0; i--) {
            // loop goes backwards but because we use a stack we will evaluate it forwards

            std::shared_ptr<Rule> definiteClause = mProgram->getRules()[i];
            std::shared_ptr<Expression> positiveLiteral = definiteClause->getLhs();

            Substitution renaming = Substitution::renameVars(definiteClause->getVariableNames());
            std::shared_ptr<Expression> renamedPositiveLiteral = renaming.applyTo(positiveLiteral);

            // if the negative and positive clause are unifiable
            std::experimental::optional<Substitution> mgu = Substitution::getMGU(negativeLiteral, renamedPositiveLiteral);
            if (mgu) {
                // resolve the clauses
                ExpsT nextQuery = unify(config.query, negativeLiteral, renaming.applyTo(definiteClause->getRhs()), *mgu);

                // update current answer
                Substitution nextAnswer = config.answer.apply(*mgu);

                // put the next configuration on the backtracking stack
                mSearchStack.push(Configuration(nextQuery, nextAnswer));
            }
        }
    }

    return std::experimental::optional<Substitution>();
}

bool contains(const ExpsT& exps, std::shared_ptr<Expression> exp) {
    for (std::shared_ptr<Expression> it : exps) {
        if (*it == *exp) {
            return true;
        }
    }
    return false;
}

ExpsT Interpreter::unify(const ExpsT& query, std::shared_ptr<Expression> resolvedLiteral, const ExpsT& definiteClause, const Substitution& mgu) const {
    ExpsT result;

    for (int i = 0; i < (int) definiteClause.size(); i++) {
        std::shared_ptr<Expression> exp = mgu.applyTo(definiteClause[i]);
        if (!contains(result, exp)) {
            result.push_back(exp);
        }
    }
    for (int i = 0; i < (int) query.size(); i++) {
        if (query[i] != resolvedLiteral) {
            std::shared_ptr<Expression> exp = mgu.applyTo(query[i]);
            if (!contains(result, exp)) {
                result.push_back(exp);
            }
        }
    }

    return result;
}