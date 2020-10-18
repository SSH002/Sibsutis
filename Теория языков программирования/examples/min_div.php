<?php
    $num1 = 12;
    $num2 = 18;
    $min = $num1 < $num2 ? $num1 : $num2;

    #1-nd task
    for($i = $min; $i > 0; $i--) { 
        if($num1 % $i == 0 && $num2 % $i == 0) { 
            $min = $i;
            break;  
        }
    }

    printf("Min_Div: %d\n", $min); 
?>
