;1 
(print(cadr(car(cadr'((1) ((2 *) (3 (4)))) )))) 
;2 
;������� list �������� ��� ��������� � ������ 
(print(list '(+ 1 2) '() '(+ 1))) 
;3 
;�� 
(print(cons '(1 2 ((3))) nil)) 
;�� 
(print(list '(1 2 ((3))))) 
;4 
;�������� ���������� �������� ������ 
(defun delete_end(param) 
	(cond
		((null (cdr param)) nil)
		(t (cons (car param) (delete_end (cdr param))))
	)
) 
;������ 2-�� � ��������� �������� ������� 
(defun 2nd_lost_changer (param) 
	(append(append (car param) (last param)) (append (cddr(delete_end param)) (cadr param)))
)
(print(2nd_lost_changer '((a) (b) c d e f g)))