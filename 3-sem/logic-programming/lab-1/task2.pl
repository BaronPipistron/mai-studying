% Вторая часть задания - реляционное представление предметной области

/*
Variant 2
*/

:- ['task1.pl'].
:- ['two.pl'].

/*
Task 1
Напечатать средний балл для каждого предмета
*/

task1 :-
    findall(X, grade(_, _, X, _), Subject1),
    setof(X, my_member(X, Subject1), Subject),
    print_AVG(Subject).

% Сумма элементов числового списка
% (список, сумма)
my_sumlist([], 0).
my_sumlist([H|T], Sum) :-
    my_sumlist(T, TSum),
    Sum is H + TSum.

find_AVG(Subject, Avg) :-
    findall(Mark, grade(_, _, Subject, Mark), Marks),
    my_sumlist(Marks, Sum),
    my_length(Marks, Length),
    Avg is Sum / Length.

print_AVG([]).
print_AVG([Subject|T]) :-
    find_AVG(Subject, Avg),
    format('AVG for Subject ~w: ~w~n', [Subject, Avg]),
    print_AVG(T).

/*
Task 2
Для каждой группы, найти количество не сдавших студентов
*/

task2 :-
    findall(X, grade(X, _, _, _), Group1),
    setof(X, my_member(X, Group1), Group),
    print_group_loosers(Group).

find_group_loosers(Group, Count) :-
    findall(X, grade(Group, X, _, 2), LooserList),
    my_length(LooserList, Count).

print_group_loosers([]).
print_group_loosers([Group|T]) :-
    find_group_loosers(Group, Count),
    format('Failed students in the group ~w: ~w~n', [Group, Count]),
    print_group_loosers(T).

/*
Task 3
Найти количество не сдавших студентов для каждого из предметов
*/

task3 :-
    findall(X, grade(_, _, X, _), Subject1),
    setof(X, my_member(X, Subject1), Subject),
    print_subject_loosers(Subject).

find_subject_loosers(Subject, Count) :-
    findall(X, grade(_, X, Subject, 2), LooserList),
    my_length(LooserList, Count).

print_subject_loosers([]).
print_subject_loosers([Subject|T]) :-
    find_subject_loosers(Subject, Count),
    format('Students who did not pass the ~w: ~w~n', [Subject, Count]),
    print_subject_loosers(T).