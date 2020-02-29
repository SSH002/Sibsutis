%������� 11.
%������� 1.
%������ ����� ����� ��������� �� ��� ������: �� ������ ��������� � �������� ���������.
%��������, [1, -2, 3, 5, -4]-> [1, 3, 5], [-2, -4].

odd_even_div([], []) --> [].
odd_even_div([Even | ET], OT) -->
    [Even], {Even mod 2 =:= 0}, odd_even_div(ET, OT).
odd_even_div(ET, [Odd | OT]) -->
    [Odd], {Odd mod 2 =:= 1}, odd_even_div(ET, OT).

%������ �������
?-phrase(odd_even_div(ET, OT), [8, 9, 2, -1, 6, -9]).
