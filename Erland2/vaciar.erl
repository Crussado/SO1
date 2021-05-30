-module(vaciar).
-export([vaciarCasilla/0]).

vaciarCasilla() ->
    receive
        Msj -> vaciarCasilla()
    after 2000 -> ok
    end.
