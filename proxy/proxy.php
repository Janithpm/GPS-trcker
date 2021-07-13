<?php

require 'firebase/src/firebaseLib.php';

$deviceId = $_GET["deviceId"];
$lat = $_GET["lat"];
$lng = $_GET["lng"];

const DEFAULT_URL = "https://gpstracker-e452b-default-rtdb.firebaseio.com"; // <---- 
const DEFAULT_TOKEN = "4S9dVPPLQCIpVlQ4QNKtjVTsQ21FjVZTRrhFHhbI"; // <----  
$PATH = '/Data/'.$deviceId;

date_default_timezone_set("Asia/Kolkata");

$deviceData = array(
    'latitude' => $lat,
    'longitude' => $lng,
    'dateTime' => date('Y-m-d H:i:s')
);

$firebase = new \Firebase\FirebaseLib(DEFAULT_URL, DEFAULT_TOKEN);
$firebase->update($PATH, $deviceData);

// print("posted")


?>