#include "Substitution.h"

Substitution::Substitution() 
{
}

Substitution Substitution::apply(Substitution other) {
    return *this;
}

std::experimental::optional<Substitution> Substitution::getMGU(std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
    // TODO
    return std::experimental::optional<Substitution>();
}