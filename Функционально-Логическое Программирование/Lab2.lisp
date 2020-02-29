;Бригада №7
;Задание 7
(defun last_insert (ins lst)
    (cond ((null lst) (list ins))
        ((> (car lst) ins) (cons ins lst))
        (t (cons (car lst) (last_insert ins (cdr lst))))
    )
)
(print(last_insert 70 '(0 3 3 6 909)))

;Задание 17
(defun del_even (lst) 
    (cond
        ((< (length lst) 2) lst)
        (t (cons (car lst) (del_even (cddr lst))))
    )
)
(print(del_even '(-2 6 s -1 4 f 0 z x r)))

;Задание 27
(defun deep_search (X)
    (cond
        ((atom X) 0)
        (t
            (   
                (lambda (e1 e2) (if (> e1 e2) e1 e2))
                (1+ (deep_search (car X)))
                (deep_search (cdr X))
            )
        )
    )
)
(print(deep_search '((((1))) 2 (3 4))))