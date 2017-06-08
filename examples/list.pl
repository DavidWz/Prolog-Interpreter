smaller(o, s(X)).
smaller(s(X), s(Y)) :- smaller(X, Y).

smallerEqual(X, X).
smallerEqual(X, Y) :- smaller(X, Y).

isSorted(nil).
isSorted(ls(X, nil)).
isSorted(ls(X, ls(Y, XS))) :- smallerEqual(X, Y), isSorted(ls(Y, XS)).

bubble(nil, nil).
bubble(ls(X, nil), ls(X, nil)).
bubble(ls(X, ls(Y, XS)), ls(X, YS)) :- smallerEqual(X, Y), bubble(ls(Y, XS), YS).
bubble(ls(X, ls(Y, XS)), ls(Y, YS)) :- smaller(Y, X), bubble(ls(X, XS), YS).

bubbleSort(XS, XS) :- isSorted(XS).
bubbleSort(XS, ZS) :- bubble(XS, YS), bubbleSort(YS, ZS).