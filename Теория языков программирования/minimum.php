<?php
    $answer2 = 2147483647;
    $arr = array(110, 172, 510, 183, 66, 410, 77, 32, 20, 81);

    //2-nd task
    for($k = 0; $k < 10; ++$k) {
        if($arr[$k] < $answer2) {
            $answer2 = $arr[$k];
        }
    }

    printf("Minimum: %d\n", $answer2);
?>

