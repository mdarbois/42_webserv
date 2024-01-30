<?php
        $dir = "./uploads/";
        
        $files = scandir($dir);
        
        foreach ($files as $file) {
            // Exclude current directory (.) and parent directory (..)
            if ($file != "." && $file != "..") {
                echo "<li>$file</li>";
            }
        }
       
?>
