% 1. Написать предикат, который разбивает числоваой список по двум числам
% на три списка: меньше меньшего введённого числа, от меньшего введённого 
% числа до большего введённого числа, больше большего введённого числа.

splite(List, A, B) :-
	%read(List), read(A), read(B),
    splite_prepare(A, B, Min, Max),
	splite_rec(List, Min, Max, List1, List2, List3),
	write(List1), write(List2), writeln(List3).

splite_prepare(A, B, Min, Max) :- A > B, !, Max = A, Min = B; Max = B, Min = A.

splite_rec([], _A, _B, [], [], []).
       
splite_rec([Head|Tail], A, B, [Head|List1], List2, List3) :-
	Head < A, !,splite_rec(Tail, A, B, List1, List2, List3).
splite_rec([Head|Tail], A, B, List1, [Head|List2], List3) :-
	Head =< B, !, splite_rec(Tail, A, B, List1, List2, List3).
splite_rec([Head|Tail], A, B, List1, List2, [Head|List3]) :-
	!, splite_rec(Tail, A, B, List1, List2, List3).

% 2. Написать предикат, фкоторый формирует список из номеров максимальных элементов числового списка.

max(List) :-
	%read(List),
    max_list(List, Maximum), max_rec(List, B, Maximum, 1), writeln(B).

max_rec([], [], _, _).
max_rec([Head|Tail], [Numb|B], Maximum, Numb) :-
	Head =:= Maximum, !, N1 is Numb + 1, max_rec(Tail, B, Maximum, N1).
max_rec([Head|Tail], B, Maximum, Numb) :-
	Head =\= Maximum, !, N1 is Numb + 1, max_rec(Tail, B, Maximum, N1).

% 3. Написать предикат, который формирует список из наиболее часто встречающихся элементов списка.

frequency(List):-
	%read(List),
	msort(List, SList),
    cou(SList, _, ResList), write(ResList).
	cou([], 0, []).
	cou([Head|B], K, [Head]) :-
		foo([Head | B], Head, K, List), cou(List, MX, _), K > MX, !.
	cou([Head | B], MX, List1) :-
		foo([Head | B], Head, K, List), cou(List, MX, List1), K < MX, !.
	cou([Head | B], MX, [Head | List1]) :-
		foo([Head | B], Head, K, List), cou(List, MX, List1), K =:= MX, !.

	foo([], _, 0, []) :- !.
	foo([Head | B], X, 0, [Head | B]) :-
        Head =\= X, !.
	foo([Head | B], X, K, B1) :- 
        Head =:= X,  foo(B, X, K1, B1), K is K1 + 1.
    
:-splite([2, 5, 7, 8, 6, 9, 14, 10, 3, 22], 9, 4).
:-max([2, 5, 7, 6, 4, 7, 3, 7, 7]).
:-frequency([12, 12, 5, 5, 3, 4, 4, 4, 3, 12, 1, 5]).