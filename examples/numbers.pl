pred(s(X), X).

smaller(o, s(X)).
smaller(s(X), s(Y)) :- smaller(X, Y).

smallerEqual(X, X).
smallerEqual(X, Y) :- smaller(X, Y).

plus(X, o, X).
plus(X, s(Y), s(Z)) :- plus(X, Y, Z).

times(X, o, o).
times(X, s(Y), Z) :- times(X, Y, U), plus(U, X, Z).

power(X, o, s(o)).
power(X, s(Y), Z) :- power(X, Y, U), times(U, X, Z).

minus(X, o, X).
minus(X, s(Y), Z) :- minus(X, Y, U), pred(U, Z).

div(X, Y, o, X) :- smaller(X, Y).
div(X, Y, s(Z), R) :- smallerEqual(Y, X), minus(X, Y, A), div(A, Y, Z, R).

divisible(X, Y) :- div(X, Y, Z, o).
notDivisible(X, Y) :- div(X, Y, Z, R), smaller(o, R).

divisors(X, XS) :- divisrs(X, XS, s(o)).
divisrs(X, [], I) :- smaller(X, I).
divisrs(X, [I|XS], I) :- smallerEqual(I, X), divisible(X, I), divisrs(X, XS, s(I)).
divisrs(X, [Y|XS], I) :- smallerEqual(I, X), notDivisible(X, I), divisrs(X, [Y|XS], s(I)).

isPrime(X) :- divisors(X, [s(o), X]).