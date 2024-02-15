<?php
$filePath = getenv('filePath');
echo $filePath;
if (!empty($filePath)) {
    // Check if the file exists
    if (file_exists($filePath)) {
        // Get the file permissions
        $permissions = fileperms($filePath);

        // Check if the file is writable
        if ($permissions & 0x0080) {
            echo "User has write permission on the file.";
        } else {
            echo "User does not have write permission on the file.";
        }
    } else {
        echo "File does not exist.";
    }
}
?>