-module(clientes).
-export([start/1, cliente/1]).

cliente(Pbro) ->
    brodcast:registrar(Pbro),
    brodcast:hablar(Pbro, "Holi"),
    receive
        Msj -> io:format("~p~n", [Msj])
    end.

start(Pbrodcast) ->
    spawn(clientes, cliente, [Pbrodcast]),
    spawn(clientes, cliente, [Pbrodcast]),
    spawn(clientes, cliente, [Pbrodcast]),
    ok.
