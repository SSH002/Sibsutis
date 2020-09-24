% Вариант 11.
% Задание 2.
% Найдите в файле все слова максимальной длины.
% Сформируйте новый файл из найденных слов.

%Предикат преобразования заданной строки в список слов
str_to_list_words(S, [Hw | Tn]):-
    front_token(S, W, S2), not(S2 = " "), !,
    Hw = W, str_to_list_words(S2, Tn).
    str_to_list_words(_, []).

%Предикат, который выделяет слова из строки0.
front_token(S, W, L1):-
    atom_chars(S, L), %строка > список символов
    append(ListW, [' ' | List1], L), !,
    atom_chars(W, ListW),
    atom_chars(L1, List1).
front_token(S, W, L1):-
    atom_chars(S, L), %строка > список символов
    append(ListW, ['.' | List1], L), !,
    atom_chars(W, ListW),
    atom_chars(L1, List1).

%Предикат формирования списка длин слов по списку этих слов
arr_len_word([], []):- !.
arr_len_word([H | T], [Hn | Tn]):-
    atom_length(H, LenW),
    Hn = LenW,
    arr_len_word(T, Tn).

%Предикат нахождения максимума в списке (длин заданных слов)
maximum([X], X).
maximum([H | T], H):-maximum(T, M), H > M, !.
maximum([_ | T], M):-maximum(T, M).

%Предикат формирования нового списка путем удаления из заданного списка слов длины, отличной от заданной величины
list_of_max_words([], _, []):- !.
list_of_max_words([H | T], X, [Hn | Tn]):-
    atom_length(H, LenW), %длина строки
    LenW = X, !,
    Hn = H,
    list_of_max_words(T, X, Tn).
list_of_max_words([_ | T], X, Ln):- list_of_max_words(T, X, Ln).

%Предикат для записи строки в файл
read_from_file(Filename, S):-
    open(Filename, read, Input),
    read(Input, S),
    close(Input).

%Предикат для записи строки в файл
write_to_file(Filename, S):-
    open(Filename, write, Out),
    write(Out, S),
    close(Out).

%предикат для запуска программы
%Input('c:/input_file.txt').
%Output('c:/output_file.txt').
%run(Input, Output):-
max_len_words(S):-
    write("Список слов:"), nl,
    %read_from_file(Input, S),
    str_to_list_words(S, Lw), %формирование списка Lw из слов строки S
    write(Lw), nl,
    arr_len_word(Lw, Ll), %нахождение длин слов и занесение их в список
    maximum(Ll, MaxLen), %нахождение максимальной из длин слов
    write("Длина наиболее длинного слова: "), write(MaxLen), nl,
    list_of_max_words(Lw, MaxLen, Ln), %формирования списка слов макс.длины
    %atomics_to_string(Ln, " ", Sn), %список > строка
    %write_to_file('Output, Sn), nl, %запись строки самых длинных слов в файл
    write(Ln).

%Строка для запуска
%max_len_words("red orange yellow green blue violet indigo.").