<?php
   
    $dir = "./uploads/";
    echo "Directory: $dir<br>";
            $files = scandir($dir);
            echo "Files: " . implode(", ", $files) . "<br>";
        
            $htmlResponse = "";
    
            // Iterate through the files and add them to the HTML response
            foreach ($files as $file) {
                if ($file != '.' && $file != '..') {
                    $htmlResponse .= "<li>$file</li>";
                }
            }
            
            echo $htmlResponse;
?>
