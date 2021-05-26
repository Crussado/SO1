-module(anillo).
-export([start/3, mensaje/0, crearProcesos/2]).

mensaje() ->
    receive
        {0, Msj, Xs, Ys} ->
            fin;
        {M, Msj, [X | Xs], Ys} ->
            io:format("~p~n", [Msj]),
            X ! {M - 1, Msj, Xs, Ys},
            mensaje();
        {M, Msj, [], Ys} ->
            [X | Xs] = Ys,
            io:format("~p~n", [Msj]),
            X ! {M - 1, Msj, Xs, Ys},
            mensaje()
        after 
            1000 -> ok
    end.

crearProcesos(0, Xs) ->
    Xs;
crearProcesos(M, Xs) -> 
    Pid = spawn(anillo, mensaje, []),
    crearProcesos(M - 1, [Pid | Xs]).

start(M, N, Msj) ->
    [X | Xs] = crearProcesos(N, []),
    X ! {M, Msj, Xs, [X | Xs]},
    fin.