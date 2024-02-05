
<?php

$answer1 = getenv('answer1');
$answer2 = getenv('answer2');
$answer3 = getenv('answer3');
$answer4 = getenv('answer4');

$color1 = $color2 = $color3 = $color4 = "red";
if ($answer1 == 'Leberknoedel')
    $color1 = "green";
if ($answer2 == 'Semmelknoedel')
    $color2 = "green";
if ($answer3 == 'Germknoedel')
    $color3 = "green";
if ($answer4 == 'Kartoffelkloesse') 
    $color4 = "green";
$htmlResponse = "<html>
<head>
<meta charset=\"UTF-8\">
<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">
<title>Knodel Game</title>
<style>
     body {
        display: flex;
        flex-direction: column;
        align-items: center;
        justify-content: center;
        height: 100vh; /* 100% viewport height */
        margin: 0; /* Remove default body margin */
    }
    .grid-container {
        display: grid;
        grid-template-columns: repeat(2, 1fr); /* Two columns with equal width */
        gap: 20px; /* Gap between grid items */
    }

    /* Style for each grid item */
    .grid-item {
        border: 1px solid #ccc; /* Border for visual separation */
        padding: 20px; /* Padding for inner content */
        text-align: center; /* Center text */
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;

    }

    /* Style for the image */
    .grid-item img {
        max-width: 100%; /* Ensure the image doesn't exceed the container width */
        height: 250px;
        object-fit: cover;
        display: block; /* Remove extra space beneath the image */
        margin-bottom: 10px; /* Add some space below the image */
    }

    /* Style for the text box */
    .grid-item input[type=\"text\"] {
        width: 100%; /* Take up the full width of the container */
        padding: 8px; /* Padding for input box */
    }
    .div-button{
        text-align: center; /* Center text */
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;

    }
    .submit-button {
        margin-top: 20px; /* Add some space above the submit button */
        padding: 10px; /* Padding for the button */
        cursor: pointer;
        justify-content: center;
        align-items: center;
    }

</style>
</head>
<body>
<h1>Welcome in our Knoedel game</h1>

<h2>Would you be able to guess which Knoedel it is</h2>
<form action=\"./game.php\" method=\"GET\">
<div class=\"grid-container\">
<!-- First grid item -->
<div class=\"grid-item\">
    <img src=\"../1024px-Leberknoedelsuppe.jpg\" alt=\"Image 1\">
    <label for=\"answer1\"></label>
    <input style=\"background-color:$color1\" type=\"text\" name=\"answer1\" placeholder=\"$answer1\" required>
</div>

<!-- Second grid item -->
<div class=\"grid-item\">
    <img src=\"../Semmelknoedel.jpg\" alt=\"Image 2\">
    <label for=\"answer2\"></label>
    <input style=\"background-color:$color2\" type=\"text\" name=\"answer2\" placeholder=\"$answer2\" required>
</div>

<!-- Third grid item -->
<div class=\"grid-item\">
    <img src=\"../Germknoedel.jpg\" alt=\"Image 3\">
    <label for=\"answer3\"></label>
    <input style=\"background-color:$color3\" type=\"text\" name=\"answer3\" placeholder=\"$answer3\" required>
</div>

<!-- Fourth grid item -->
<div class=\"grid-item\">
    <img src=\"../Kloesse.jpg\" alt=\"Image 4\">
    <label for=\"answer4\"></label>
    <input style=\"background-color:$color4\" type=\"text\" name=\"answer4\" placeholder=\"$answer4\" required>
</div>
</div>
<div class=\"div-button\">
    <input type=\"submit\" class=\"submit-button\" value=\"Submit\">
</div>
</form>

<a href=\"/\">Back to Home</a>
</body>
</html>";
echo $htmlResponse;
?>