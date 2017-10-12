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
if ($email!="")header('Location: account.php');
?>
<!DOCTYPE html>
<html>
<head>
    <link rel="stylesheet" href="f1.css" type="text/css"/>
    <link rel="shortcut icon" href="img/logo.ico"/>
    <title>HMS Hospital</title>
    <script src="script/function.js"></script>
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


<?php
function extracredential($temp,$fit)
{
    $pos=strpos($temp,'=');
    return substr($temp, $pos+1,strlen($temp)-$pos-2);
}
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
if(isset($_POST["submit"]))
{
    $conn=mysqli_connect($host,$user,$pass,$db) or die('could not connect');
    $sqlquery = "Use ".$db.";";
    if ($conn->query($sqlquery) === FALSE) {echo "Error : " . $conn->error;die('selecting to database:');}
    $r=$_POST['verify'];
    $code=(($r)%100000+($r)%10000+($r)%1000+($r)%100+($r)%10)%100000;
    echo "enter code ".$_POST['ecode'];
    echo "systemcode ".$code;
    if($code==$_POST['ecode'])
    {
        $sqlquery='UPDATE account SET state="active" WHERE temp="'.$_POST['verify'].'";';
        $conn->query($sqlquery);
        header('Location: congratulation.php');
    }
    else header('Location: verification.php?verify='.$_POST['verify']);
}
?>



<div class="display_noti">
    <div class="image_panel" id="display_image">
        <form method="post" action="verification.php" name="verification" onsubmit="">
            <div class="image_panel_veri" style="margin:30px;line-height: 1.0;">
                &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<b>Yipee!! You almost created your account</b><br><br>
                Now you need to verify your email address. We've sent an email to  to verify your address that email contain a verification key enter in given and submit.
                <br>
            </div>
            <div class="image_panel_left">
                Enter Verification Code<br><br><br>
            </div>
            <div class="image_panel_right">
                <input class="signup_top" type="text" name="ecode" id="name" length="5" maxlength="5" value="">&nbsp;
            </div>
            <div class="image_panel_bottom">
                <?php echo '<input type="hidden" name="verify"value="'.$_GET['verify'].'" >'; ?>
                <input name="submit" type="submit" value="Submit">
                <?php echo '<h1 id="form_message" style="color: #DF0000; font-size: 25px;" >'.$msg; ?> </h1>
            </div>
        </form>
    </div>
    <div class="noti_panel">
        <div class="noti_panel_top">
            <ul>
                <li><a><b>Notification</b></a></li>
            </ul>
        </div>
        <div class="noti_panel_bottom">
            You can also set the width of , and remove the width of , as they will take up the full width available when
            displayed as block elements. This will produce the same result as our previous example:
        </div>
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
