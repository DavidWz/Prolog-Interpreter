pred(s(X), X).

smaller(o, s(X)).
smaller(s(X), s(Y)) :- smaller(X, Y).

smallerEqual(X, X).
smallerEqual(X, Y) :- smaller(X, Y).

min(X, Y, X) :- smallerEqual(X, Y).
min(X, Y, Y) :- smaller(Y, X).

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

root(X, R, Z) :- power(Z, R, X).

divisible(X, Y) :- div(X, Y, Z, o).
notDivisible(X, Y) :- div(X, Y, Z, R), smaller(o, R).

divisors(X, XS) :- divisrs(X, XS, s(o)).
divisrs(X, [], I) :- smaller(X, I).
divisrs(X, [I|XS], I) :- smallerEqual(I, X), divisible(X, I), divisrs(X, XS, s(I)).
divisrs(X, [Y|XS], I) :- smallerEqual(I, X), notDivisible(X, I), divisrs(X, [Y|XS], s(I)).

isPrime(X) :- divisors(X, [s(o), X]).



simplify(q(X, Y), Z) :- min(X, Y, M), simp(q(X, Y), M, Z).
simp(X, s(o), X).
simp(q(X, Y), I, q(Zx, Zy)) :- div(X, I, Zx, o), div(Y, I, Zy, o).
simp(X, s(I), Z) :- simp(X, I, Z).

plus(q(Tx, Bx), q(Ty, By), Z) :- times(Bx, By, B), times(Tx, By, Tl), times(Ty, Bx, Tr), plus(Tl, Tr, T), simplify(q(T, B), Z).
minus(q(Tx, Bx), q(Ty, By), Z) :- times(Bx, By, B), times(Tx, By, Tl), times(Ty, Bx, Tr), minus(Tl, Tr, T), simplify(q(T, B), Z).
times(q(Tx, Bx), q(Ty, By), Z) :- times(Tx, Ty, T), times(Bx, By, B), simplify(q(T, B), Z).
div(X, q(Ty, By), Z) :- times(X, q(By, Ty), Z).
inv(q(T, B), I) :- simplify(q(B, T), I).
power(q(Tx, Bx), q(Et, Eb), Z) :- power(Tx, Et, Te), root(Te, Eb, T), power(Bx, Et, Be), root(Be, Eb, B), simplify(q(T, B), Z).

int(q(T, B), I, R) :- div(T, B, I, R).