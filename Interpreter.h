#pragma once

#include "Common.h"
#include "Substitution.h"
#include "ast/Program.h"
#include "ast/Expression.h"
#include <stack>

class Configuration {
public:
    ExpsT query; 
    Substitution answer;

    Configuration(const ExpsT& query, const Substitution& answer) :
        query(query),
        answer(answer)
    {
    }

    Configuration(const ExpsT& query) : 
        query(query),
        answer()
    {
    }
};

class Interpreter {
private:
    std::shared_ptr<Program> mProgram;
    std::stack<Configuration> mSearchStack;
    std::set<std::string> mOrigVars;

public:
    Interpreter(std::shared_ptr<Program> program);

    /**
     * Sets the query that should be evaluated.
     */
    void init(const ExpsT& query);

    /**
     * Evaluates the query and returns the next found answer.
     */
    std::experimental::optional<Substitution> nextAnswer();

    /**
     * Whether there might be a next answer.
     */
    bool hasNext();

private:
    /**
     * Applies mgu to query and definite clause, unifies both and removes the resolved literal.
     */
    static ExpsT unify(const ExpsT& query, std::shared_ptr<Expression> resolvedLiteral, const ExpsT& definiteClause, const Substitution& mgu);

    /**
     * Checks whether exps contains an expression equal to exp.
     */
    static bool contains(const ExpsT& exps, std::shared_ptr<Expression> exp);

    /**
     * Retrieves all variables from all expressions in exps.
     */
    static std::set<std::string> getVariableNames(const ExpsT& exps);
};