-module(listas).
-export([play/0]).

min([]) -> cedo;
min([X]) -> X;
min([X | Xs]) -> 
    M = min(Xs),
    if
        M < X -> M;
    true ->
        X
    end.

max([]) -> cedo;
max([X]) -> X;
max([X | Xs]) ->
    M = max(Xs),
    if
        M > X -> M;
    true ->
        X
    end.

tupla([]) -> cedo;
tupla([X | Xs]) ->
    A = [X | Xs],
    {min(A), max(A)}.

map2([], F) -> [];
map2([X | Xs], F) -> [F(X) | map2(Xs, F)].

sumar(X) -> X+1.

play() ->
    A = [1,2,3,4,9,-11],
    F = fun(X) -> X+1 end,
    B = map2(A, F),
    B.
