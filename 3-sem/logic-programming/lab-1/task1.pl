% Первая часть задания - предикаты работы со списками

/*
    Task 1:
    List predicates
*/

% Длина списка
% (список, длина)

my_length([], 0).
my_length([_|L], N) :-
    my_length(L, M),
    N is M + 1.


% Принадлежность элемента списку
% (элемент, список)

my_member(X, [X|_]).
my_member(X, [_|L]) :-
    my_member(X, L).


% Конкатенация списков
% (список_1, список_2, список_1 + список_2)

my_append([], X, X).
my_append([A|X], Y, [A|Z]) :-
    my_append(X,Y,Z).


% Удаление элемента из списка
% (элемент, список, список без элемента)

my_remove(X, [X|T], T).
my_remove(X, [Y|T], [Y|Z]) :-
    my_remove(X, T, Z).


% Перестановки элементов списка
% (список, перестановка)

my_premute([], []).
my_permute(L, [X|T]) :-
    my_remove(X, L, R),
    my_permute(R, T).


% Подсписки списка
% (подсписок, список)

my_sublist(S, L) :-
    my_append(_, L1, L),
    my_append(S, _, L1).

/*
    Task 2:
    Variant 10
    Вставка элемента в список на указанную позицию
*/

% 1 - С использованием стандартных предикатов

standart_insert(Element, 0, [H|T], ResList) :-
    my_append([Element], [H], NewList),
    my_append(NewList, T, ResList).

standart_insert(Element, 0, [], ResList) :-
    my_append([Element], [], ResList).

standart_insert(Element, Pos, [H|T], ResList) :-
    Pos > 0, 
    CurPos is Pos - 1,
    standart_insert(Element, CurPos, T, L),
    my_append([H], L, ResList).

% 2 - Без использования стандартных предикатов

custom_insert(Element, 0, [H|T], ResList) :-
    T \= [],
    ResList = [Element, H|T].

custom_insert(Element, 0, [], ResList) :-
    ResList = [Element].

custom_insert(Element, Pos, [H|T], ResList) :-
    Pos > 0,
    CurPos is Pos - 1,
    custom_insert(Element, CurPos, T, L),
    ResList = [H|L].

/*
    Task 3:
    Variant 15
    Вычисление позиции первого отрицательного элемента в списке
*/

% 1 - С использованием стандартных предикатов

standart_first_negative_element_pos(List, Pos) :-
    my_append(Start, [X|_], List),
    X < 0,
    my_length(Start, Pos).

% 2 - Без использования стандартных предикатов

custom_first_negative_element_pos(List, Pos) :-
    first_negative_element_pos_handler(List, 0, Pos).

first_negative_element_pos_handler([H|_], Pos, Pos) :-
    H < 0.

first_negative_element_pos_handler([_|T], CurPos, Pos) :-
    NewPos is CurPos + 1,
    first_negative_element_pos_handler(T, NewPos, Pos).

/*
    Task 4:
    Привести какой-нибудь содержательный пример совместного использования предикатов
*/

% самое содержательное что смог придумать
% вставить элемент перед первым отрицательным числом и после него
insert_before_and_after_first_neg_element(Element, List, ResList) :-
    standart_first_negative_element_pos(List, Pos), 
    custom_insert(Element, Pos, List, ResList1),
    custom_insert(Element, Pos + 2, ResList1, ResList).

% перемещение элемента в списке
move_element(Element, PosToMove, List, ResList) :-
    my_remove(Element, List, ResList1),
    custom_insert(Element, PosToMove, ResList1, ResList).