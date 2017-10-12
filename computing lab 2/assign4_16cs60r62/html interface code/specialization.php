<?php
session_start();
$email="";
$name="";
$rand="";
$type="";
if(isset($_SESSION['email']) && !empty($_SESSION['email']))
{
$email=$_SESSION["email"];
$name=$_SESSION["name"];
$rand=$_SESSION['rand'];
$type=$_SESSION['type'];
}
?>
<!DOCTYPE html>
<html>
<head>
    <link rel="stylesheet" href="f1.css" type="text/css"/>
    <link rel="shortcut icon" href="img/logo.ico"/>
    <title>HMS Hospital</title>
    <script src="script/function.js"></script>
    <link rel="stylesheet" href="jquery-ui.css">
    <link rel="stylesheet" href="style.css">
    <script src="script/jquery-1.12.4.js"></script>
    <script src="script/jquery-ui.js"></script>
</head>
<body>
<div class="hearder">
    <div class="logo">
        <img src="img/logo.jpg" height="10%" width="40%"/>
    </div>
    <div class="title">
        <h1>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="/" id="logoLink">HMS HOSPITAL</a></h1>
    </div>
    <div class="control" id="control">
        <?php
            if($email=="")
            {
                echo '<div class="logindiv">';
        echo '<form name="loginform" action="login.php" method="post" >';
        echo '<input type="text" name="username" id="username" size="50" style="width:200px;" placeholder="Enter Email" oninput="oninputevent(event)"  >';
        echo '<input type="password" name="password" id="password" size="20" style="width:150px;" placeholder="Password" oninput="oninputevent(event)">';
        echo '<input type="submit" name="login" value="Login">';
        echo '</form>';
        echo '</div>';
    echo '<div class="signupdiv">';
    echo '<form action="signup.php" method="post">';
        echo '<input type="submit" value="sign up">';
        echo '</form>';
    echo '</div>';
    echo '<div class="forget">';
    echo '<a href="forgetlogin.php" class="home">Forget Login Password</a>';
    echo '</div>';
    echo '<div class="message" id="control_message">';
    echo '</div>';
    }
    else
    {
    echo '<div class="logoutdiv">';
    echo '<img src="imguploads/'.$email.'" width="80px" height="80px"><br>';
    echo 'Welcome '.$name.'<br>';
    echo 'Your Email '.$email.'<br>';
    echo '<form name="logoutform" action="logout.php" method="post" >';
        echo '<input type="submit" name="logout" value="Logout">';
        echo '</form>';
    echo '</div>';
    }
    ?>
</div>
</div>




<div class="navigation">
    <ul>
        <li ><a href="index.php" class="home"><b>Home</b></a></li>
        <li><a href="aboutus.php"><b>About Us</b></a></li>
        <li><a href="facilities.php"><b>Facilities</b></a></li>
        <li><a href="doctors.php"><b>Doctors</b></a></li>
        <li><a href="contactus.php"><b>Contact Us</b></a></li>
        <?php if($email!="") echo '<li><a href="account.php"><b>My Account</b></a></li>'; ?>
    </ul>
</div >






<div class="display_noti">
    <div class="accountbar">
        <ul>
            <?php if($type=="patient") echo '<li><a href="addappointment.php"><b>Add Appointment</b></a></li>';?>
            <li><a href="delappointment.php"><b>View & Delete Appointment</b></a></li>
            <li><a href="viewdiagnosis.php"><b>View Diagnosis</b></a></li>
            <?php if($type=="doctor") echo '<li><a href="adddiagnosis.php"><b>Add Diagnosis</b></a></li>'; ?>
            <?php if($type=="doctor") echo '<li><a href="specialization.php"><b>Specialization</b></a></li>'; ?>
            <li><a href="scheduleoperation.php"><b>Schedule Operation</b></a></li>
        </ul>
    </div>
    <div class="status">
        <form name="viewdiagnosisform" method="post" action="specialization.php">
            <?php
                        function extracredential($temp,$fit)
            {
                $pos=strpos($temp,'=');
                return substr($temp, $pos+1,strlen($temp)-$pos-2);
            }
            function gettimestamp()
            {
                return round(microtime(true) * 1000);
            }
            $msg="";
            $host="";
            $pass="";
            $db="";
            $user="";
            $myfile = fopen("credentials/sql.txt", "r") or die("Unable to open file!");
            $user=fgets($myfile);
            $user=extracredential($user,"username");
            $pass=fgets($myfile);
            $pass=extracredential($pass,"password");
            $host=fgets($myfile);
            $host=extracredential($host,"host");
            $db=fgets($myfile);
            $db=extracredential($db,"database");
            fclose($myfile);
            $conn=mysqli_connect($host,$user,$pass,$db) or die('could not connect');
            $sqlquery = "Use ".$db.";";
            if ($conn->query($sqlquery) === FALSE) {echo "Error: " . $conn->error;die('unable to select database data');}
            if(isset($_POST["viewdiagnosissubmit"])&& $_POST['spec']!="")
            {
                $sqlquery='insert into speciality values("'.$email.'","'.$_POST['spec'].'" );';
                if ($conn->query($sqlquery) === FALSE) {echo "Error: " . $conn->error;die('could not insert update diagnosis ');}
                echo "Diagnosis successfully updated";
            }
            $sqlquery='select * from speciality where email="'.$email.'";';
            $dvalue="";
            if($result = $conn->query($sqlquery))
            {
            echo '<br><br>';
            echo '<h6 style="padding-left: 5%;line-height: 120%;font-size: 140%;"> Available Specialization';
            if($result->num_rows > 0)
            {
            while($row = $result->fetch_array())
            {
                $dvalue=$dvalue.$row['specialization'].'<br>';
            }
            }
            else
            {
            echo "<br>No records matching your query were found.";
            }
            echo '</h6>';
            }
            else
            {
            echo "ERROR: Could not able to execute $sql. " . $conn->error;
            }
            echo $dvalue;
            ?>
        </form>
        <form name="addappointmentfinalizeform" action="specialization.php" method="post">
            <input name="spec" type="text"  length="" value="">
            <input name="viewdiagnosissubmit" type="submit" value="ADD Specialization" style="width: 18%">
        </form>
    </div>
</div>
<hr>







<div class="footer">
    <div class="footer_left">
        <b style="font-size: 200%;"><u>HMS Hospital</u></b>
        <li ><a href="index.php" class="home"><b>Home</b></a></li>
        <li><a href="aboutus.php"><b>About Us</b></a></li>
        <li><a href="facilities.php"><b>Facilities</b></a></li>
        <li><a href="doctors.php"><b>Doctors</b></a></li>
        <li><a href="contactus.php"><b>Contact Us</b></a></li>
    </div>
    <div class="footer_centre">
        <b style="font-size: 200%;"><u>Partners</u></b>
        <li ><a href="http://apolloari.com/" class="home"><b>Apollo Research & Innovations</b></a></li>
        <li><a href="http://www.aherf.org/"><b>Research Foundation</b></a></li>
        <li><a href="https://www.apollohospitals.com/research/apollo-medical-journal"><b>Apollo Medical Journal</b></a></li>
        <li><a href="https://www.maxhealthcare.in"><b>Max Smart Super Specialty Hospital</b></a></li>
        <li><a href="www.peerlesshospital.com"><b>Peerless Hospital</b></a></li>
    </div>
    <div class="footer_right">
        <b>Get HMS Route </b>
        <br>
        <br>
        <div id="map" style="width:100%;height:350px;"></div>
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
    </div>
</div>
</body>
</html>
