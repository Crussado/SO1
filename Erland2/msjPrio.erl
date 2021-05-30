-module(msjPrio).
-export([start/0, llegoMsj/0]).

llegoMsj() ->
    receive
        {msj1, Msj}->
            io:format("~p~n", [Msj]),
            receive
                {msj2, Msj2}->
                    io:format("~p~n", [Msj2])
            end
    end.

start() ->
    A = spawn(msjPrio, llegoMsj, []), 
    A ! {msj2, "Yo pimedo cadajo"},
    A ! {msj1, "Ño, yo pimedo cadajo"}.
    