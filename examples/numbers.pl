minus(X, o, X).
minus(X, Y, Z) :- minus(A, B, Z), pred(X, A),pred(Y, B).
pred(s(s(X)), s(Y)) :- pred(s(X), Y).
pred(s(X), X).