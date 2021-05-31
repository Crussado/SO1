-module(server).
-define(PUERTO, 8008).
-define(COMANDO_INVALIDO, "Comando no valido.\0").
-define(NOMBRE_INVALIDO, "Dicho nombre ya existe.\0").
-define(USUARIO_INVALIDO, "Dicho usuario no existe.\0").
-export([start/0,fin/1,receptor/1,echoResp/2,gestionarMapa/1,mandarMsjGlobal/3,
generarNombre/1, enviarMsjError/2]).

%% start: Crear un socket, y ponerse a escuchar.
start()->
    %% register(servidor, self()),
    {ok, Socket} = gen_tcp:listen(?PUERTO
                                 , [ binary, {active, false}]),
    spawn(?MODULE,receptor, [Socket]),
    Pid = spawn(?MODULE, gestionarMapa, [maps:new()]),
    register(agenda, Pid).

fin(Socket) ->
    gen_tcp:close(Socket).

enviarMsjError(Socket, Msj) ->
    gen_tcp:send(Socket, Msj).

gestionarMapa(Mapa) ->
    receive
        {agregar, Socket, Pid} ->
            Nombre = generarNombre(Mapa),
            NewMapa = maps:put(Nombre, Socket, Mapa),
            Pid ! {ok, Nombre},
            gestionarMapa(NewMapa);

        {global, Nombre, Msj} ->
            Keys = maps:keys(Mapa),
            mandarMsjGlobal(Keys, Mapa, Nombre ++ ": " ++ Msj ++ "\0"),
            gestionarMapa(Mapa);

        {cambiar, Nombre, NewNombre, Pid} ->
            Socket = maps:get(Nombre, Mapa),
            case maps:is_key(NewNombre, Mapa) of 
                false ->
                    MapaR = maps:remove(Nombre, Mapa),
                    NewMapa = maps:put(NewNombre, Socket, MapaR),
                    Pid ! NewNombre,
                    gestionarMapa(NewMapa);
                true -> 
                    enviarMsjError(Socket, ?NOMBRE_INVALIDO),
                    Pid ! Nombre,
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
                    gen_tcp:send(Socket, "[" ++ NombreOrigen ++ "]: " ++ Msj ++ "\0"),
                    gestionarMapa(Mapa)
            end;

        {salir, Nombre} ->
            Socket = maps:get(Nombre, Mapa),
            fin(Socket),
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
    Nro = rand:uniform(50),
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
            io:format("Nos vamos a mimir."),
            agenda ! fin,
            exit(normal);
        {error, Reason} ->
            io:format("Falló la espera del client por: ~p~n",[Reason])
    end,
    receptor(Socket).
    %% end.

%% echoResp: atiende al cliente.
echoResp(Socket, Nombre) ->
    case gen_tcp:recv(Socket, 0) of
        {ok, Packet} ->
            MsjBin = binary:split(Packet, <<0>>),
            MsjList = lists:map(fun binary:bin_to_list/1, MsjBin),
            Msj = hd(MsjList),
            Datos = string:lexemes(Msj, " "),
            if 
                Datos == [] ->
                    echoResp(Socket, Nombre);
                true -> 
                    ok
            end,
            case lists:nth(1, Datos) of 
                "/exit" ->
                    agenda ! {salir, Nombre};
                "/nickname" ->
                    if
                        (length(Datos) /= 2) -> 
                            enviarMsjError(Socket, ?COMANDO_INVALIDO),
                            echoResp(Socket, Nombre);
                    true->
                        agenda ! {cambiar, Nombre, lists:nth(2, Datos), self()},
                        receive 
                            Name -> echoResp(Socket, Name)
                        end
                    end;
                "/msj" -> 
                    if
                        (length(Datos) < 3) ->
                            enviarMsjError(Socket, ?COMANDO_INVALIDO);
                    true ->   
                        agenda ! {susurrar, Nombre, lists:nth(2, Datos), string:join(tl(tl(Datos)), " ")}
                    end,
                    echoResp(Socket, Nombre);
                _ ->
                    agenda ! {global, Nombre, Msj},
                    echoResp(Socket, Nombre)
            end;
        {error, closed} ->
            io:format("El cliente cerró la conexión~n"),
            agenda ! {salir, Nombre}
    end.
