<?php
// Directory path
$directory = './html/uploads';

// Initialize an array to store file names
$files = array();
// Open the directory
if ($handle = opendir($directory)) {
    // Loop through the directory
    while (false !== ($file = readdir($handle))) {
        // Exclude current directory (.) and parent directory (..)
        if ($file != "." && $file != "..") {
            // Add the file to the array
            $files[] = $file;
        }
    }
    // Close the directory handle
    closedir($handle);
}

// Convert the array to JSON
$json = json_encode($files);

// Set the response header to indicate JSON content
//header('Content-Type: application/json');

// Output the JSON
echo $json;
?>