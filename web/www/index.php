<?php
$servername = "localhost";
$username = "ardueco";
$password = "";
$database = "my_ardueco";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $database);

// Check connection
if (!$conn) {
    die("Connection failed: " . mysqli_connect_error());
}

$rand = array('0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f');

?>

<!DOCTYPE html>
<html>

<head>
    <meta name="viewport" content="initial-scale=1.0, user-scalable=no" />
    <meta http-equiv="content-type" content="text/html; charset=UTF-8" />
    <title>Using MySQL and PHP with Google Maps</title>
    <style>
        /* Always set the map height explicitly to define the size of the div
       * element that contains the map. */
        #map {
            height: 100%;
        }

        /* Optional: Makes the sample page fill the window. */
        html,
        body {
            height: 100%;
            margin: 0;
            padding: 0;
        }
    </style>
</head>


<body>
    <div id="map"></div>

    <script>
        var customLabel = {
            restaurant: {
                label: 'R'
            },
            bar: {
                label: 'B'
            }
        };

        function initMap() {
            // This example creates a 2-pixel-wide red polyline showing the path of
            // the first trans-Pacific flight between Oakland, CA, and Brisbane,
            // Australia which was made by Charles Kingsford Smith.

            var map = new google.maps.Map(document.getElementById('map'), {
                zoom: 12,
                center: {
                    lat: 45.417,
                    lng: 11.867
                },
                mapTypeId: 'terrain'
            });

            <?

            $route_sql = "SELECT DISTINCT route_id FROM marker";
            $route_result = mysqli_query($conn, $route_sql);

            if (mysqli_num_rows($route_result) > 0) {
                while ($route_row = mysqli_fetch_assoc($route_result)) {

                    $sql = "SELECT * FROM marker WHERE route_id=" . $route_row["route_id"];
                    $result = mysqli_query($conn, $sql);

                    $markers = array();

                    $flightPlanCoordinates =  "var flightPlanCoordinates_" . $route_row["route_id"] . " = [
                    ";
                    if (mysqli_num_rows($result) > 0) {
                        while ($row = mysqli_fetch_assoc($result)) {
                            $flightPlanCoordinates = $flightPlanCoordinates . "
                                { lat : " . $row["latitude"] . " , lng : " . $row["longitude"] . " },";
                            $marker = "
                            var marker_" . $row["id"] . " = new google.maps.Marker({
                                position: { lat : " . $row["latitude"] . " , lng : " . $row["longitude"] . "},
                                map: map,
                                title: 'Route #" . $row["route_id"] . "',
                                label : ''
                            });
                            
                            marker_" . $row["id"] . ".description = new google.maps.InfoWindow({
                                  content:'" . $row["air"] . "'
                            });
                            
                            google.maps.event.addListener(marker_" . $row["id"] . ", 'click', function(){
                                this.description.setPosition(this.getPosition());
                                  this.description.open(map);
                            });
                            ";
                            array_push($markers, $marker);
                        }
                    }
                    $flightPlanCoordinates = $flightPlanCoordinates . "
                    ];
                    ";

                    $color = '#' . $rand[rand(0, 15)] . $rand[rand(0, 15)] . $rand[rand(0, 15)] . $rand[rand(0, 15)] . $rand[rand(0, 15)] . $rand[rand(0, 15)];

                    foreach ($markers as $marker) {
                        echo $marker;
                    }

                    $flightPath = "var flightPath_" . $route_row["route_id"] . " = new google.maps.Polyline({
                        path: flightPlanCoordinates_" . $route_row["route_id"] . ",
                        geodesic: true,
                        strokeColor: '" . $color . "',
                        strokeOpacity: 1.0,
                        strokeWeight: 2
                    });";
                    
                    echo "
                    " . $flightPlanCoordinates;

                    echo "
                    " .  $flightPath;

                    echo "
                        flightPath_" . $route_row["route_id"] . ".setMap(map);";
                }
            }
            ?>

            // var flightPlanCoordinates_1 = [{
            // 		lat: -27.467,
            // 		lng: 153.027
            // 	},
            // 	{
            // 		lat: 37.772,
            // 		lng: -122.214
            // 	},
            // 	{
            // 		lat: -18.142,
            // 		lng: 178.431
            // 	},
            // 	{
            // 		lat: 21.291,
            // 		lng: -157.821
            // 	}
            // ];
            // var flightPath_1 = new google.maps.Polyline({
            // 	path: flightPlanCoordinates_1,
            // 	geodesic: true,
            // 	strokeColor: '#FF0000',
            // 	strokeOpacity: 1.0,
            // 	strokeWeight: 2
            // });

            // flightPath_1.setMap(map);

            // var marker = new google.maps.Marker({
            // 	position: { lat: -25.363, lng: 131.044 },
            // 	map: map,
            // 	title: 'Hello World!'
            // });

        }
    </script>
    <script async defer src="https://maps.googleapis.com/maps/api/js?key=YOUR_MAPS_KEY&callback=initMap"></script>
</body>

</html>

<? mysqli_close($conn); ?>