plus(b0, b0, b0, b0, b0).
plus(b0, b1, b0, b1, b0).
plus(b1, b0, b0, b1, b0).
plus(b1, b1, b0, b0, b1).
plus(b0, b0, b1, b1, b0).
plus(b0, b1, b1, b0, b1).
plus(b1, b0, b1, b0, b1).
plus(b1, b1, b1, b1, b1).

inv(b0, b1).
inv(b1, b0).

inv(i8(X7, X6, X5, X4, X3, X2, X1, X0), i8(Y7, Y6, Y5, Y4, Y3, Y2, Y1, Y0)) :- 
    inv(X0, Y0), inv(X1, Y1), inv(X2, Y2), inv(X3, Y3), inv(X4, Y4), inv(X5, Y5), inv(X6, Y6), inv(X7, Y7).

shiftr(i8(X7, X6, X5, X4, X3, X2, X1, X0), i8(b0, X7, X6, X5, X4, X3, X2, X1)).
shiftl(i8(X7, X6, X5, X4, X3, X2, X1, X0), i8(X6, X5, X4, X3, X2, X1, X0, b0)).

zero(i8(b0, b0, b0, b0, b0, b0, b0, b0)).
one(i8(b0, b0, b0, b0, b0, b0, b0, b1)).

isNegative(i8(b1, X6, X5, X4, X3, X2, X1, X0)).
isPositive(i8(b0, X6, X5, X4, X3, X2, X1, X0)).

plus(i8(X7, X6, X5, X4, X3, X2, X1, X0), i8(Y7, Y6, Y5, Y4, Y3, Y2, Y1, Y0), i8(Z7, Z6, Z5, Z4, Z3, Z2, Z1, Z0)) :- 
    plus(X0, Y0, b0, Z0, O1),
    plus(X1, Y1, O1, Z1, O2),
    plus(X2, Y2, O2, Z2, O3),
    plus(X3, Y3, O3, Z3, O4),
    plus(X4, Y4, O4, Z4, O5),
    plus(X5, Y5, O5, Z5, O6),
    plus(X6, Y6, O6, Z6, O7),
    plus(X7, Y7, O7, Z7, O8).

minus(X, Y, Z) :- plus(Y, Z, X).

negate(X, Y) :- plus(Y, X, i8(b0, b0, b0, b0, b0, b0, b0, b0)).

succ(X, Y) :- plus(X, i8(b0, b0, b0, b0, b0, b0, b0, b1), Y).
pred(X, Y) :- minus(X, i8(b0, b0, b0, b0, b0, b0, b0, b1), Y).

equal(X, X).
less(X, Y) :- minus(X, Y, Z), isNegative(Z).
lessEqual(X, Y) :- minus(Y, X, Z), isPositive(Z).
greater(X, Y) :- less(Y, X).
greaterEqual(X, Y) :- lessEqual(Y, X).
notEqual(X, Y) :- less(X, Y).
notEqual(X, Y) :- less(Y, X).

timesBit(X, b0, i8(b0, b0, b0, b0, b0, b0, b0, b0)).
timesBit(X, b1, X).

timesPositive(X, i8(Y7, Y6, Y5, Y4, Y3, Y2, Y1, Y0), Z) :- 
    timesBit(X, Y0, P0),
    timesBit(X, Y1, i8(P17, P16, P15, P14, P13, P12, P11, P10)),
    timesBit(X, Y2, i8(P27, P26, P25, P24, P23, P22, P21, P20)),
    timesBit(X, Y3, i8(P37, P36, P35, P34, P33, P32, P31, P30)),
    timesBit(X, Y4, i8(P47, P46, P45, P44, P43, P42, P41, P40)),
    timesBit(X, Y5, i8(P57, P56, P55, P54, P53, P52, P51, P50)),
    timesBit(X, Y6, i8(P67, P66, P65, P64, P63, P62, P61, P60)),
    timesBit(X, Y7, i8(P77, P76, P75, P74, P73, P72, P71, P70)),
    plus(P0, i8(P16, P15, P14, P13, P12, P11, P10, b0), P1),
    plus(P1, i8(P25, P24, P23, P22, P21, P20,  b0, b0), P2),
    plus(P2, i8(P34, P33, P32, P31, P30,  b0,  b0, b0), P3),
    plus(P3, i8(P43, P42, P41, P40,  b0,  b0,  b0, b0), P4),
    plus(P4, i8(P52, P51, P50,  b0,  b0,  b0,  b0, b0), P5),
    plus(P5, i8(P61, P60,  b0,  b0,  b0,  b0,  b0, b0), P6),
    plus(P6, i8(P70,  b0,  b0,  b0,  b0,  b0,  b0, b0),  Z).

times(X, Y, Z) :- isPositive(X), isPositive(Y), timesPositive(X, Y, Z).
times(X, Y, Z) :- isNegative(X), isPositive(Y), negate(X, NX), times(NX, Y, NZ), negate(NZ, Z).
times(X, Y, Z) :- isPositive(X), isNegative(Y), negate(Y, NY), times(X, NY, NZ), negate(NZ, Z).
times(X, Y, Z) :- isNegative(X), isNegative(Y), negate(X, NX), negate(Y, NY), times(NX, NY, Z).

divPositive(X, Y, i8(b0, b0, b0, b0, b0, b0, b0, b0), X) :- less(X, Y).
divPositive(X, Y, Z, R) :- lessEqual(Y, X), minus(X, Y, A), divPositive(A, Y, Z0, R), succ(Z0, Z).

div(X, Y, Z, R) :- isPositive(X), isPositive(Y), divPositive(X, Y, Z, R).
div(X, Y, Z, R) :- isNegative(X), isPositive(Y), negate(X, NX), div(NX, Y, NZ, NR), negate(NZ, Z), negate(NR, R).
div(X, Y, Z, R) :- isPositive(X), isNegative(Y), negate(Y, NY), div(X, NY, NZ, NR), negate(NZ, Z), negate(NR, R).
div(X, Y, Z, R) :- isNegative(X), isNegative(Y), negate(X, NX), negate(Y, NY), div(NX, NY, Z, R).
