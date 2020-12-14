<?php
    $str1_size = 12;
    $str2_size = 4;
    $flag = 0;

    $str1 = array('S', 'u', 'm', 's', 'u', 'm', 'm', 'u', 's', 'm', 'u', 's');
    $str2 = array('u', 'm', 'm', 'u');

    #3-rd task
    for($i = 0; $i < $str1_size; ++$i) {
        if($str1[$i] == $str2[$flag]) {
            $flag++;
        }
        if($flag == $str2_size) {
            printf("Substr: TRUE\n");
            break;
        }
    }

    if($flag != $str2_size) {
        printf("Substr: FALSE\n");
    }
?>
