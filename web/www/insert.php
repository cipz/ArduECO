<?php

$servername = "YOUR_SERVER";
$username = "YOUR_USERNAME";
$password = "YOUR_PASSWORD";
$database = "YOUR_DATABASE";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $database);

// Check connection
if (!$conn) {
    die(" CONNECTION_ERR " . mysqli_connect_error());
}
echo " CONNECTION_OK ";

$DD = substr($_POST["dt"], 0, 2);
$MM = substr($_POST["dt"], 2 ,2);
$YYYY = "20" . substr($_POST["dt"], 4 ,2);
$hh = substr($_POST["dt"], 7 ,2);
$mm = substr($_POST["dt"], 9, 2);
$ss = substr($_POST["dt"], 11, 2);

$new_date = date_create_from_format('Y-m-d H:i:s', $YYYY . "-" . $MM . "-" . $DD . " " . $hh  . ":" . $mm  . ":" . $ss);
$new_date = date_format($new_date, 'Y-m-d H:i:s');

$sql = "INSERT INTO `marker`(`latitude`, `longitude`, `time`, `route_id`, `air`, `topic`) VALUES (" . $_POST["lt"] . ", " . $_POST["lg"] .", '" . $new_date . "', " . $_POST["id"] . ", " . $_POST["air"] . ", '" . $_POST["tpc"] . "');";

if (mysqli_query($conn, $sql)) {
    echo " RECORD_UPDATE_OK ";
} else {
    echo " RECORD_UPDATE_ERR: " . $sql . "<br>" . mysqli_error($conn);
}

mysqli_close($conn);