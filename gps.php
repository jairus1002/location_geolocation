<?php

$hostname = "localhost";
$username = "root";
$password = "";
$database = "gps";

// Function to connect to the database
function connectDatabase() {
    global $hostname, $username, $password, $database;
    // Connect to MySQL database
    $conn = mysqli_connect($hostname, $username, $password, $database);
    // Check if connection was successful
    if (!$conn) {
        die("Connection failed: " . mysqli_connect_error());
    }
    return $conn;
}

// Function to retrieve the latest coordinates from the database along with the previous ones
function getCoordinates() {
    $conn = connectDatabase();
    // Initialize latitude and longitude arrays
    $coordinates = array();
    // SQL query to fetch latitude and longitude from the database
    $sql = "SELECT latitude, longitude FROM los ORDER BY id DESC LIMIT 10"; // Fetching 10 most recent coordinates
    $result = mysqli_query($conn, $sql);
    if (mysqli_num_rows($result) > 0) {
        // Fetch the latitude and longitude values
        while ($row = mysqli_fetch_assoc($result)) {
            $coordinates[] = array("lat" => $row["latitude"], "lng" => $row["longitude"]);
        }
    }
    // Close the database connection
    mysqli_close($conn);
    // Return latitude and longitude coordinates as JSON
    return json_encode($coordinates);
}

// Check if data is received via POST method
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // Decode JSON data
    $data = json_decode(file_get_contents("php://input"), true);
    // Extract latitude and longitude from JSON data
    $latitude = $data["latitude"];
    $longitude = $data["longitude"];
    // Insert new coordinates into the database
    insertCoordinates($latitude, $longitude);
} else {
    // Handle GET request to retrieve coordinates
    echo getCoordinates();
}

?>
