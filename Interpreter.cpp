#include "Interpreter.h"
#include "Substitution.h"
#include "ast/Expression.h"
#include <algorithm>

Interpreter::Interpreter(std::shared_ptr<Program> program) :
    mProgram(program)
{
}

std::experimental::optional<Substitution> Interpreter::evaluate(ExpsT query) {
    ExpsT negativeClause = query;
    Substitution answer;

    // try to resolve the query until we get the empty clause
    while (negativeClause.size() > 0) {
        std::shared_ptr<Expression> negativeLiteral = negativeClause[0];
    
        // try to resolve the negative clause with a definite program clause
        bool isUnifiable = false;
        for (int i = 0; i < (int) mProgram->getRules().size(); i++) {
            std::shared_ptr<Rule> definiteClause = mProgram->getRules()[i];
            std::shared_ptr<Expression> positiveLiteral = definiteClause->getLhs();

            // if the negative and positive clause are unifiable
            std::experimental::optional<Substitution> mgu = Substitution::getMGU(negativeLiteral, positiveLiteral);
            if (mgu) {
                isUnifiable = true;

                // resolve the clauses
                auto it = std::find(negativeClause.begin(), negativeClause.end(), negativeLiteral);
                negativeClause.erase(it);
                negativeClause = unify(negativeClause, definiteClause->getRhs(), *mgu);

                // update answer
                answer.apply(*mgu);

                break;
            }
            else {
                isUnifiable = false;
            }
        }

        if (!isUnifiable) {
            // return empty indicating not resolvable
            return std::experimental::optional<Substitution>();
        }
    }

    return answer;
}


ExpsT Interpreter::unify(ExpsT left, ExpsT right, Substitution mgu) {
    // TODO
    return left;
}