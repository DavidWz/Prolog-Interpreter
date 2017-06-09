#pragma once

#include "Common.h"
#include "Substitution.h"
#include "ast/Program.h"
#include "ast/Expression.h"

class Interpreter {
private:
    std::shared_ptr<Program> mProgram;

public:
    Interpreter(std::shared_ptr<Program> program);

    /**
     * Evaluates the given query under the program of this interpreter.
     * @param query the query that should be evaluated
     * @return the answer substitution if the query is true, empty otherwise
     */
    std::experimental::optional<Substitution> evaluate(const ExpsT& query) const;

private:
    /**
     * Applies mgu to left and right expressions and returns a list containing both. 
     */
    ExpsT unify(const ExpsT& left, std::shared_ptr<Expression> resolvedLiteral, const ExpsT& right, const Substitution& mgu) const;

    /**
     * Evaluates the given query assuming a previous answer substitution.
     * @param query the query
     * @param currentAnswer the current answer substitution
     * @return the final answer substitution if the query is true, empty otherwise 
     */
    std::experimental::optional<Substitution> evaluate(const ExpsT& query, const Substitution& currentAnswer) const;
};