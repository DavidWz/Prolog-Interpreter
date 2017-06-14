smaller(o, s(X)).
smaller(s(X), s(Y)) :- smaller(X, Y).

smallerEqual(X, X).
smallerEqual(X, Y) :- smaller(X, Y).

isSorted([]).
isSorted([X]).
isSorted([X, Y | XS]) :- smallerEqual(X, Y), isSorted([Y | XS]).

bubble([], []).
bubble([X], [X]).
bubble([X, Y | XS], [X | YS]) :- smallerEqual(X, Y), bubble([Y | XS], YS).
bubble([X, Y | XS], [Y | YS]) :-      smaller(Y, X), bubble([X | XS], YS).

bubbleSort(XS, XS) :- isSorted(XS).
bubbleSort(XS, ZS) :- bubble(XS, YS), bubbleSort(YS, ZS).

duplicates(XS) :- bubbleSort(XS, YS), dupl(YS).
dupl([X, X | Y]).
dupl([X, Y | XS]) :- dupl([Y | XS]).

countNumbers(XS, AS) :- bubbleSort(XS, YS), count(YS, AS, o).
count([], [], X).
count([I], [s(o)], I).
count([I | XS], [s(A) | AS], I) :- count(XS, [A | AS], I).
count([X | XS], [o | AS], I) :- smaller(I, X), count([X | XS], AS, s(I)).