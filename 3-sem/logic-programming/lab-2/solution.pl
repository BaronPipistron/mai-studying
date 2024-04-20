/*
Variant 9

Один из пяти братьев разбил окно. Андрей сказал: Это или Витя, или Толя. 
Витя сказал: Это сделал не я и не Юра. Дима сказал: Нет, один из них сказал правду, 
а другой неправду. Юра сказал: Нет, Дима ты не прав. Их отец, которому, 
конечно можно доверять, уверен, что не менее трех братьев сказали правду.
Кто разбил окно? 
*/

my_member(X, [X|_]).
my_member(X, [_|L]) :-
    my_member(X, L).

% Andrey
statement("Andrey", "Vitya").
statement("Andrey", "Tolya").

% Vitya
statement("Vitya", Guilty) :-
    Guilty \= "Vitya",
    Guilty \= "Yura".

% Dima
statement("Dima", Guilty) :-
    (statement("Andrey", Guilty), not(statement("Vitya", Guilty)));
    (statement("Vitya", Guilty), not(statement("Andrey", Guilty))).

% Yura
statement("Yura", Guilty) :-
    not(statement("Dima", Guilty)).

% Check statements to truth
check_statements([], _).
check_statements([H|T], Guilty) :-
    statement(H, Guilty),
    check_statements(T, Guilty).

% Find who lied
solution(Guilty) :-
    my_member(Guilty, ["Andrey", "Vitya", "Dima", "Yura", "Tolya"]),
    my_member(Lied, ["Andrey", "Vitya", "Dima", "Yura"]),
    delete(["Andrey", "Vitya", "Dima", "Yura"], Lied, Speakers),
    check_statements(Speakers, Guilty),
    !.

% Print result
print_ans :-
    solution(Guilty),
    format('~w break the window ~n', [Guilty]).