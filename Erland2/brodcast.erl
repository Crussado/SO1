-module(brodcast).
-export([start/0, hablar/1, cerrar/0, registrar/0, brodcastMain/1]).

start() ->
    P = spawn(brodcast, brodcastMain, [[]]),
    register(bro, P).

hablar(Msj) ->
    bro ! {msj, Msj}.

cerrar() ->
    bro ! {salir}.


registrar() ->
    bro ! {reg, self()}.

brodcastMain(Xs) ->
    receive
        {reg, Pid} ->
            brodcastMain([Pid | Xs]);
        {msj, Msj} ->
            lists:foreach(fun (Pid) -> Pid ! Msj end, Xs),
            brodcastMain(Xs);
        {salir} ->
            ok
    end.