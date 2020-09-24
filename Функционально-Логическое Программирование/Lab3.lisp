;Бригада №7
;Задание 2
(defun is_subset (a b)
    (cond
        ((null a) t) 
        ((member (car a) b) (is_subset (cdr a) b))
        (t nil)
    )
)  
(print(is_subset '(x y) '(a c v x z y)))
(print(is_subset '(x y i) '(a c v x z y)))

;Задание 6
(defun in_predicate (a l)
    (cond
        ((null l) nil)
        ((eq a (car l)) t)
        (t (in_predicate a (cdr l)))
    )
)

(defun difference (a b)
    (cond
        ((null a) nil)
        ((null b) a)
        ((in_predicate (car a) b) (difference (cdr a) b))
        (t (cons (car a)(difference (cdr a) b)))
    )
)
(print(difference '(a b c d e) '(b c f g)))

;Задание 9
(defun change (func lst) 
    (cond
        ((null lst) nil) 
        ((funcall func (car lst))(cons (list '* (car lst)) (change func (cdr lst))))
        (t (cons (car lst) (change func (cdr lst))))
    )
)
(print(change #'evenp '(1 2 3 4 5 6)))
(print(change #'(lambda (x) (> x 0)) '(-1 4 3 -6 7 -7)))