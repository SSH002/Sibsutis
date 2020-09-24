%1. Написать предикат, который печатает все нечётные числа из диапазона в порядке убывания.
even(X):- X mod 2 =:= 0.

print_uneven(X, Y):-
	not(even(Y)),
	write(Y), write(", "), fail.

print_uneven(X, Y):-
	Y \== X, Y1 is Y - 1, print_uneven(X, Y1).

uneven(Z, Y):-
    %read(X),
    %read(Y),
    print_uneven(Z, Y).

%2. Написать предикат, который находит сумму всех целых чисел из диапазона.
sum_prepare(X, Y, X1, Y1) :-
    X > Y, !,
    Y1 = X, X1 = Y;
    Y1 = Y, X1 = X.

summation(X1, Y2, Sum) :-
	S is X1 + Sum,
	X1 \== Y2,
	X2 is X1 + 1,
	summation(X2, Y2, S).

summation(X, X, Sum) :- write(Sum).

sum(X, Y) :-
		%read(X),
		%read(Y),
        sum_prepare(X, Y, X1, Y1),
		Y2 is Y1 + 1,
		summation(X1, Y2, 0).

%3. Написать предикат, который находит минимальную цифру в целом числе.
min_digit(0, Min) :- write(Min).

min_digit(X, Min) :-
	D is X mod 10,
	Y is X div 10,
	D < Min,
	min_digit(Y, D).

min_digit(X, Min) :-
	Y is X div 10,
	min_digit(Y, Min).

min_digit(X) :-
	%read(X),
	X \= 0,
	Y is abs(X),
	min_digit(Y, 9).

%4. Написать предикат, который находит числа Фибоначчи по их номерам. 
%   Запрос номера и нахождение соответствующего числа Фибоначчи. 
%   Числа Фибоначчи определяются по следующим формулам:
%   F(0)=1, F(1)=1, F(i)=F(i-2)+F(i-1) (i=2, 3, 4, ...).

calc_fibonacci(0, Y) :- Y is 0, !.
calc_fibonacci(1, Y) :- Y is 1, !.

calc_fibonacci(X, Y) :-
	X1 is X - 2,
	X2 is X - 1,
	calc_fibonacci(X1, Y1),
	calc_fibonacci(X2, Y2),
	Y is Y1 + Y2.

fibonacci(X) :-
	%repeat,
	%read(X),
	(X < 0, !; calc_fibonacci(X, Y), write(Y)).

:-uneven(3, 14).
:-nl, sum(0, 1000), nl.
:-min_digit(7548), nl.
:-fibonacci(6).