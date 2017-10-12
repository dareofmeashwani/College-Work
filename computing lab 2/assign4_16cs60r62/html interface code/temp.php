<?php
session_start();
?>
<!DOCTYPE html>
<html>

<body>
<?php
echo $_SESSION["email"];
echo $_SESSION["name"];
print_r($_SESSION);
?>
<br>
<br>
<br>
<div id="map" style="width:90%;height:400px;"></div>

<script>
    function myMap() {
        var myCenter = new google.maps.LatLng(22.31635,87.29999);
        var mapCanvas = document.getElementById("map");
        var mapOptions = {center: myCenter, zoom: 16};
        var map = new google.maps.Map(mapCanvas, mapOptions);
        var marker = new google.maps.Marker({position:myCenter});
        marker.setMap(map);
        var infowindow = new google.maps.InfoWindow({
            content: "HMS Hospital Kharagpur!"
        });
        infowindow.open(map,marker);
    }
</script>
<script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyBNbtr-tF7i4GnfMaeFkddQh70yGiGjoEI&callback=myMap"></script>
<!--
To use this code on your website, get a free API key from Google.
Read more at: https://www.w3schools.com/graphics/google_maps_basic.asp
-->

</body>
</html>
