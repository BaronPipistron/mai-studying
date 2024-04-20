/*
Variant 2

Три миссионера и три каннибала хотят переправиться с левого берега реки на правый. Как это сделать
за минмальное число шагов, если в их распоряжении имеется трехместная лодка и ни при каких обстоятельствах
(в лодке или на берегу) миссионеры не должны оставаться в меньшинстве
*/

% Запишем все возможные варианты переправ через на правый берег.
% M1, M2 - количество миссионеров на берегу, C1, C2 - количество каннибалов.
% [M, C, where] - состояние количества миссионеров (M) и каннибалов (С) на левом берегу и расположения лодки

% в одну сторону
move([M1, C1, left], [M2, C1, right], '1 миссионер переплывает на правый берег') :-
	M1 > 0,  M2 is M1 - 1.
move([M1, C1, left], [M2, C1, right], '2 миссионера переплывают на правый берег') :-
	M1 > 1,  M2 is M1 - 2.
move([M1, C1, left], [M2, C1, right], '3 миссионера переплывают на правый берег') :-
	M1 = 3,  M2 is 0.
move([M1, C1, left], [M1, C2, right], '1 каннибал переплывает на правый берег') :-
	C1 > 0,  C2 is C1 - 1.
move([M1, C1, left], [M1, C2, right], '2 каннибала переплывают на правый берег') :-
	C1 > 1,  C2 is C1 - 2.
move([M1, C1, left], [M1, C2, right], '3 каннибала переплывают на правый берег') :-
	C1 = 3,  C2 is 0.
move([M1, C1, left], [M2, C2, right], '1 миссионер и 1 каннибал переплывают на правый берег') :-
	M1 > 0,  C1 > 0,  M2 is M1 - 1,  C2 is C1 - 1.
move([M1, C1, left], [M2, C2, right], '2 миссионера и 1 каннибал переплывают на правый берег') :-
	M1 > 1,  C1 > 0,  M2 is M1 - 2,  C2 is C1 - 1.

% в обратную сторону
move([M1, C1, right], [M2, C1, left], '1 миссионер переплывает на левый берег') :-
	M1 < 3,  M2 is M1 + 1.
move([M1, C1, right], [M2, C1, left], '2 миссионера переплывают на левый берег') :-
	M1 < 2,  M2 is M1 + 2.
move([M1, C1, right], [M1, C2, left], '1 каннибал переплывает на левый берег') :-
	C1 < 3,  C2 is C1 + 1.
move([M1, C1, right], [M1, C2, left], '2 каннибала переплывают на левый берег') :-
	C1 < 2,  C2 is C1 + 2.
move([M1, C1, right], [M2, C2, left], '1 миссионер и 1 каннибал переплывают на левый берег') :-
	M1 < 3,  C1 < 3,  M2 is M1 + 1,  C2 is C1 + 1.
move([M1, C1, right], [M2, C2, left], '2 миссионера и 1 каннибал переплывают на левый берег') :-
	M1 < 2,  C1 < 3,  M2 is M1 + 2,  C2 is C1 + 1.
move([M1, C1, right], [M2, C1, left], '3 миссионера переплывают на левый берег') :-
	M1 = 0,  M2 is 3.
move([M1, C1, right], [M1, C2, left], '3 каннибала переплывают на левый берег') :-
	C1 = 0,  C2 is 3.

% условие на количество миссионеров (проверяет состояние после переправы лодки).
% Для упрощения условия изначально не рассматривается неподходящий по условию случай, когда на лодке перевозятся 2 каннибала и 1 миссионер.
condition([M_left, C_left, _]) :-
	(M_left >= C_left ; M_left = 0),
	M_right is 3 - M_left, C_right is 3 - C_left,
	(M_right >= C_right ; M_right = 0).

% Вспомогательный предикат для поиска
prolong([X|T], [Y, X|T]) :-
	move(X, Y, _),
	condition(Y),
	not(member(Y, [X|T])).


% Поиск в глубину, DFS
solve_DFS :-
	get_time(T),
	path_DFS([[3, 3, left]], [0, 0, right], Res),
	print_path(Res),
	write("Время: "),
	get_time(T1), DT is T1 - T, write(DT), write(" sec"), nl.

%solve_DFS :-
%	path_DFS([[3, 3, left]], [0, 0, right], Res),
%	print_path(Res).

path0_DFS(X, Y, R) :- path_DFS([X], Y, R). 
path_DFS([Y|T], Y, [Y|T]).
path_DFS(PrevPath, Y, R) :-
	prolong(PrevPath, P1),
	path_DFS(P1, Y, R).

% Поиск в ширину, BFS
solve_BFS :-
	get_time(T),
	path_BFS([ [[3, 3, left]] ], [0, 0, right], Res),
	print_path(Res),
	write("Время: "),
	get_time(T1), DT is T1 - T, write(DT), write(" sec"), nl.

%solve_BFS :-
%	path_BFS([ [[3, 3, left]] ], [0, 0, right], Res),
%	print_path(Res).

path_BFS([[Y|T]|_], Y, [Y|T]).
path_BFS([P|QI], X, R) :-
	findall(Z, prolong(P, Z), T),
	append(QI, T, QQ), !,
	path_BFS(QQ, X, R).
path_BFS([_|T], Y, R) :- path_BFS(T, Y, R).


% Вспомогательный предикат для поиска с итер. загл.
int(1).
int(M) :- int(N), M is N + 1.

% Поиск с итерационным заглублением
solve_ID :-
	get_time(T),
	int(DepLimit),
	path_ID([[3, 3, left]], [0, 0, right], Res, DepLimit),
	print_path(Res),
	write("Время: "),
	get_time(T1), DT is T1 - T, write(DT), write(" sec"), nl.
%solve_ID :-
%	int(DepLimit),
%	path_ID([[3, 3, left]], [0, 0, right], Res, DepLimit),
%	print_path(Res).

path_ID([X|T], X, [X|T], 0).
path_ID(P, Y, R, N) :- 
	N > 0,
	prolong(P, P1),
	N1 is N - 1,
	path_ID(P1, Y, R, N1).

% Печать результата поиска
print_path([_]).
print_path([Y, X|T]) :-
	print_path([X|T]),
	move(X, Y, Action),
	write(X), write(' -> '),
	write(Y), write(': '),
	write(Action), nl.