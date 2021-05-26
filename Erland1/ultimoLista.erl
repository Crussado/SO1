-module(ultimoLista).
-export([play/0]).

primerosDos(Xs) ->
    [A, B | Resto ] = Xs,
    io:format("Me llego una lista cuyos primeros dos elementos son~n"
             "\t + ~p ~n \t + ~p~n",[A, B]).

ultimo(Xs) ->
    [A| Resto ] = Xs,
    if
        Resto == [] ->
            io:format("Ultimo: ~p ~n",[A]);
        true ->
            ultimo(Resto)
    end.

ultimo2([]) -> io:format("Error~n");
ultimo2([X]) -> io:format("Ultimo: ~p~n",[X]);
ultimo2([X | Xs]) -> ultimo2(Xs);
ultimo2(A) -> io:format("No es una lista").

play() ->
    Xs = [1 , 2 ,3 , banana, manzana, pera],
    primerosDos(Xs),
    ultimo(Xs), % deberia ser `pera`
    ultimo2(Xs).
