parent(jhon, bob).
parent(marry, bob).
parent(marry, ann).
parent(bob, liz).
parent(bob, paul).
parent(bob, sam).
parent(paul, put).

man(jhon).
man(bob).
man(paul).
man(sam).

woman(marry).
woman(ann).
woman(liz).
woman(put).

child(C, P):-
    parent(P, C).
son(S, P):-
    man(S),
    parent(P, S).
daughter(D, P):-
    woman(D),
    parent(P, D).
brother(BR, U):-
    man(BR), 
    (parent(Z, BR), parent(Z, U)), BR \= U.
sister(SI, U):-
    woman(SI), 
    (parent(Z, SI), parent(Z, U)), SI \= U.
mother(M, C):-
    woman(M),
    parent(M, C).
father(F, C):-
    man(F),
    parent(F, C).
aunt(AU, N):-
    sister(AU, Z), child(N, Z).
grandchild(CC, PP):-
    child(Z, PP), child(CC, Z).
grandparent(PP, CC):-
    parent(PP, Z), parent(Z, CC).
family_successor(U):-
    child(_, U), man(U).
two_child(U):-
    child(Z, U), child(Q, U), child(Z, U), child(Q, U), Z \= Q.

parent(jhon, bob).
parent(marry, bob).
parent(marry, ann).
parent(bob, liz).
parent(bob, paul).
parent(bob, sam).
parent(paul, put).

man(jhon).
man(bob).
man(paul).
man(sam).

woman(marry).
woman(ann).
woman(liz).
woman(put).

brother(BR, U):-
    man(BR), 
    (parent(Z, BR), parent(Z, U)), BR \= U.
sister(SI, U):-
    woman(SI), 
    (parent(Z, SI), parent(Z, U)), SI \= U.
mother(M, C):-
    woman(M),
    parent(M, C).
father(F, C):-
    man(F),
    parent(F, C).
aunt(AU, N):-
    sister(AU, Z), parent(Z, N).
grandchild(CC, PP):-
    parent(PP, Z), parent(Z, CC).
family_successor(U):-
    parent(U, _), man(U).
two_child(U):-
    parent(U, Z), parent(U, Q), parent(U, Z), parent(U, Q), Z \= Q.
grandparent_all(X, Y):-parent(X, Y).
grandparent_all(X, Y):-parent(X, Z), grandparent_all(Z, Y).

:-mother(marry, bob).
:-father(jhon, bob).
:-grandchild(sam, marry).
:-sister(ann, bob).
:-brother(paul, sam).
:-aunt(ann, sam).
:-family_successor(jhon).
:-two_child(marry).

1)w:father(X, bob).
2)mother(_, bob).
3)w:sister(X, sam).
4)sister(_, liz).
5)w:brother(X, bob).
6)w:grandchild(X, marry).
7)w:grandchild(paul, X).
8)w:aunt(X, sam).
9)w:aunt(ann, _).
10)w:two_child(X).
11)w:family_successor(bob).