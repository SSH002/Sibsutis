;1 
(print(cadr(car(cadr'((1) ((2 *) (3 (4)))) )))) 
;2 
;Команда list собирает все аргументы в список 
(print(list '(+ 1 2) '() '(+ 1))) 
;3 
;Аа 
(print(cons '(1 2 ((3))) nil)) 
;Бэ 
(print(list '(1 2 ((3))))) 
;4 
;Удаление последнего элемента списка 
(defun delete_end(param) 
	(cond
		((null (cdr param)) nil)
		(t (cons (car param) (delete_end (cdr param))))
	)
) 
;Меняем 2-ой и последний элементы местами 
(defun 2nd_lost_changer (param) 
	(append(append (car param) (last param)) (append (cddr(delete_end param)) (cadr param)))
)
(print(2nd_lost_changer '((a) (b) c d e f g)))