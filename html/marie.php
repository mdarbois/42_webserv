<?php
$name = getenv('name');
$htmlResponse = "<html>
<head>
    <title>Welcome You</title>
</head>
<body>
    <h1>Welcome $name</h1>
    <h2>Happy to C you</h2>
    <a href=\"/index.html\">Back to Home</a>
</body>
</html>";

echo $htmlResponse;
?>