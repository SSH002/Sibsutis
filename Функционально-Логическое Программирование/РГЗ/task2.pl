% ������� 11.
% ������� 2.
% ������� � ����� ��� ����� ������������ �����.
% ����������� ����� ���� �� ��������� ����.

%�������� �������������� �������� ������ � ������ ����
str_to_list_words(S, [Hw | Tn]):-
    front_token(S, W, S2), not(S2 = " "), !,
    Hw = W, str_to_list_words(S2, Tn).
    str_to_list_words(_, []).

%��������, ������� �������� ����� �� ������0.
front_token(S, W, L1):-
    atom_chars(S, L), %������ > ������ ��������
    append(ListW, [' ' | List1], L), !,
    atom_chars(W, ListW),
    atom_chars(L1, List1).
front_token(S, W, L1):-
    atom_chars(S, L), %������ > ������ ��������
    append(ListW, ['.' | List1], L), !,
    atom_chars(W, ListW),
    atom_chars(L1, List1).

%�������� ������������ ������ ���� ���� �� ������ ���� ����
arr_len_word([], []):- !.
arr_len_word([H | T], [Hn | Tn]):-
    atom_length(H, LenW),
    Hn = LenW,
    arr_len_word(T, Tn).

%�������� ���������� ��������� � ������ (���� �������� ����)
maximum([X], X).
maximum([H | T], H):-maximum(T, M), H > M, !.
maximum([_ | T], M):-maximum(T, M).

%�������� ������������ ������ ������ ����� �������� �� ��������� ������ ���� �����, �������� �� �������� ��������
list_of_max_words([], _, []):- !.
list_of_max_words([H | T], X, [Hn | Tn]):-
    atom_length(H, LenW), %����� ������
    LenW = X, !,
    Hn = H,
    list_of_max_words(T, X, Tn).
list_of_max_words([_ | T], X, Ln):- list_of_max_words(T, X, Ln).

%�������� ��� ������ ������ � ����
read_from_file(Filename, S):-
    open(Filename, read, Input),
    read(Input, S),
    close(Input).

%�������� ��� ������ ������ � ����
write_to_file(Filename, S):-
    open(Filename, write, Out),
    write(Out, S),
    close(Out).

%�������� ��� ������� ���������
%Input('c:/input_file.txt').
%Output('c:/output_file.txt').
%run(Input, Output):-
max_len_words(S):-
    write("������ ����:"), nl,
    %read_from_file(Input, S),
    str_to_list_words(S, Lw), %������������ ������ Lw �� ���� ������ S
    write(Lw), nl,
    arr_len_word(Lw, Ll), %���������� ���� ���� � ��������� �� � ������
    maximum(Ll, MaxLen), %���������� ������������ �� ���� ����
    write("����� �������� �������� �����: "), write(MaxLen), nl,
    list_of_max_words(Lw, MaxLen, Ln), %������������ ������ ���� ����.�����
    %atomics_to_string(Ln, " ", Sn), %������ > ������
    %write_to_file('Output, Sn), nl, %������ ������ ����� ������� ���� � ����
    write(Ln).

%������ ��� �������
%max_len_words("red orange yellow green blue violet indigo.").