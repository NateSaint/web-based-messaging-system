<?php
    $toEval = "";
    $cmd = './parseConfig index.wpml';
    exec($cmd, $output, $status);
    
    /*Call for checkid.php to be created*/
    exec('./parseConfig checkid.wpml');

    if ($status)
        echo "Exec failed";
    else {
        $started = 0;
        for ($i = 0; $i < count($output); $i++)
        {
            if ($started == 1)
                $toEval .= $output[$i];

            if ($output[$i] == "<?php" || $output[$i] == "<?PHP")
                $started = 1;

            if ($output[$i] == "?>" && $started == 1)
            {
                eval("$toEval");
                $started = 0;
                $toEval = "";
            }

            else if ($started == 0)
                echo $output[$i];
        }
    }
?>