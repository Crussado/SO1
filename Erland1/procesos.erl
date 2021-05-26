-module(procesos).
-export([start/2, enviar/0]).

% ping(M, Msj, B) ->
%     if
%         M == 0 -> ok;
%     true ->
%         A = M - 1,
%         io:format("~p~n", [Msj]),
%         B ! {A, Msj, self()},
%         receive
%             f -> ok;
%             {X,Y} -> Ping(X, Y, B);
%         end
%     end.

% pong() ->
%     receive 
%         {0,Y,Z} -> Z ! f;
%         {X,Y,Z} ->
%             A = X - 1,
%             io:format("~p~n", [Y]),
%             if 
%                 A == 0,
%                 Z ! f;
%             true ->
%                 Z ! {A, Y}
%             end
%     end.

enviar() ->
    receive
        fin -> ok;
        {0, Msj, Pid} ->
            Pid ! fin;
        {M, Msj, Pid} ->
            io:format("~p~n", [Msj]),
            Pid ! {M - 1, Msj, self()},
            enviar()
    end.

start(M, Msj) ->
    Pid1 = spawn(procesos, enviar , []),
    Pid2 = spawn(procesos, enviar , []),
    Pid1 ! {M, Msj, Pid2}.
