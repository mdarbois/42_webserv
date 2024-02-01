
<?php
// Simulate a delay of 5 seconds
echo '<script>';
echo 'setTimeout(function() {';
echo 'document.querySelector("p").textContent = "Request processed successfully!";';
echo '}, 5000);'; // 5000 milliseconds = 5 seconds
echo '</script>';
?>