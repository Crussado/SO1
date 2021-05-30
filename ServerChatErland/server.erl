-module(server).
-define(PUERTO, 1234).
-define(COMANDO_INVALIDO, "Comando no valido.~n").
-define(NOMBRE_INVALIDO, "Dicho nombre ya existe.~n").
-define(USUARIO_INVALIDO, "Dicho usuario no existe.~n").
-export([start/0,fin/1,receptor/1,echoResp/1]).

%% start: Crear un socket, y ponerse a escuchar.
start()->
    %% register(servidor, self()),
    {ok, Socket} = gen_tcp:listen(?PUERTO
                                 , [ binary, {active, false}]),
    spawn(?MODULE,receptor, [Socket]),
    Pid = spawn(?MODULE, gestionarMapa, [maps:new()]),
    register(agenda, Pid).

fin(Socket) ->
    gen_tcp:close(Socket),
    

gestionarMapa(Mapa) ->
    receive
        {agregar, Socket, Pid} ->
            Nombre = generarNombre(Mapa),
            NewMapa = maps:put(Nombre, Socket, Mapa),
            Pid ! {ok, Nombre},
            gestionarMapa(NewMapa);

        {global, Nombre, Msj} ->
            Keys = maps:keys(Mapa),
            mandarMsjGlobal(Keys, Mapa, Nombre ++ ": " ++ Msj),
            gestionarMapa(Mapa);

        {cambiar, Nombre, NewNombre} ->
            Socket = maps:get(Nombre, Mapa),
            case maps:is_key(NewNombre, Mapa) of 
                false ->
                    MapaR = maps:remove(Nombre, Mapa),
                    NewMapa = maps:put(NewNombre, Socket, MapaR),
                    gestionarMapa(NewMapa);
                true -> 
                    enviarMsjError(Socket, ?NOMBRE_INVALIDO),
                    gestionarMapa(Mapa)
            end;
    
        {susurrar, NombreOrigen, NombreDestino, Msj} ->
            case maps:is_key(NombreDestino, Mapa) of 
                false ->
                    Socket = maps:get(NombreOrigen, Mapa),
                    enviarMsjError(Socket, ?USUARIO_INVALIDO),
                    gestionarMapa(Mapa);
                true ->
                    Socket = maps:get(NombreDestino, Mapa),
                    gen_tcp:send(Socket, "[" ++ NombreOrigen ++ "]: " ++ Msj),
                    gestionarMapa(Mapa)
            end;
        {salir, Nombre} ->
            MapaR = maps:remove(Nombre, Mapa),
            gestionarMapa(MapaR);
        fin -> 
            ok
    end.

mandarMsjGlobal([], Mapa, Msj) ->
    ok;
mandarMsjGlobal([Key | Xs], Mapa, Msj) ->
    Socket = maps:get(Key, Mapa),
    gen_tcp:send(Socket, Msj),
    mandarMsjGlobal(Xs, Mapa, Msj).

generarNombre(Mapa)->
    Nro = rand:numero(50),
    Nombre = "Cliente" ++ integer_to_list(Nro),
    case maps:is_key(Nombre, Mapa) of 
        false ->
            Nombre;
        true -> 
            generarNombre(Mapa)
    end.

%% receptor: Espera a los clientes y crea nuevos actores para atender los pedidos.
%%
receptor(Socket) ->
        case gen_tcp:accept(Socket) of
            {ok, CSocket} ->
                agenda ! {agregar, CSocket, self()},
                receive
                    {ok, Nombre} ->
                        spawn(?MODULE, echoResp,[CSocket, Nombre])
                end;
            {error, closed} ->
                io:format("Se cerró el closed, nos vamos a mimir"),
                agenda ! fin,
                exit(normal);
            {error, Reason} ->
                io:format("Falló la espera del client por: ~p~n",[Reason])
        end,
        receptor(Socket).
    %% end.

removeZeros([]) ->
    [];
removeZeros([0 | Xs]) ->
    removeZeros(Xs);
removeZeros([_X | Xs]) -> 
    [_X | removeZeros(Xs)].


enviarMsjError(Socket, Msj) ->
    gen_tcp:send(Socket, Msj).

%% echoResp: atiende al cliente.
echoResp(Socket, Nombre) ->
    case gen_tcp:recv(Socket, 0) of
        {ok, Packet} ->
            Msj = (removeZeros(binary_to_list(Packet))),
            io:format("Me llegó ~p~n", [Msj]),
            Datos = string:lexemes(Msj, " "),
            case lists:nth(1, Datos) of 
                "/exit" ->
                    agenda ! {salir, Nombre};
                "/nickname" ->
                    if
                        (length(Datos) /= 2) -> 
                            enviarMsjError(Socket, ?COMANDO_INVALIDO);
                    true->
                        agenda ! {cambiar, Nombre, lists:nth(2, Datos)}
                    end;
                "/msj" -> 
                    if
                        (length(Datos) /= 3) ->
                            enviarMsjError(Socket, ?COMANDO_INVALIDO);
                    true ->
                        agenda ! {susurrar, lists:nth(2, Datos), lists:nth(3, Datos)}
                    end;
                _ ->
                    agenda ! {global, Nombre, Msj}
            end,
            echoResp(Socket, Nombre);
        {error, closed} ->
            io:format("El cliente cerró la conexión~n")
    end.
