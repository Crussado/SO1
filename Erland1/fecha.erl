-module(fecha).
-export([fecha_actual/0, wait/1, nada/0]).
-export([cronometro/3, saludar/0]).

nada() -> io:format("Chau~n").

fecha_actual() -> 
    {Anio, Mes, Dia} = date(),
    integer_to_list(Anio) ++ "/" ++ integer_to_list(Mes) ++ integer_to_list(Dia).

wait(X) ->
    timer:apply_after(X, fecha, nada, []).

saludar() -> io:format("Holi~n").

cronometro(Periodo, Hasta, F) ->
    {ok, Tref} = timer:apply_interval(Periodo, fecha, F, []),
    timer:apply_after(Hasta, timer, cancel, [Tref]).
