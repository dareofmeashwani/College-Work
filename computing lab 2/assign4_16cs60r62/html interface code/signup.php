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
        <li><a href="index.php" class="home"><b>Home</b></a></li>
        <li><a href="aboutus.php"><b>About Us</b></a></li>
        <li><a href="facilities.php"><b>Facilities</b></a></li>
        <li><a href="doctors.php"><b>Doctors</b></a></li>
        <li><a href="contactus.php"><b>Contact Us</b></a></li>
        <?php if($email!="") echo '<li><a href="account.php"><b>My Account</b></a></li>'; ?>
    </ul>
</div>
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
function staffkey_exist($c,$e,$s)
{
    $count=0;
    $sqlquery='select count(email) as c from staff_key where email="'.$e.'" and staffkey="'.$s.'";';
    $queryresult=$c->query($sqlquery);
    if ($queryresult->num_rows > 0)
    {
        while($row = $queryresult->fetch_assoc())
        {
            $count=$count+$row['c'];
        }
    }
    if($count!=1) {return FALSE;}
    else {return TRUE;}
}
function email_exist($c,$e)
{
    $count=0;
    $sqlquery='select count(email) as c from account where email="'.$e.'";';
    $queryresult=$c->query($sqlquery);
    if ($queryresult->num_rows > 0)
    {
        while($row = $queryresult->fetch_assoc())
        {
        $count=$count+$row['c'];
        }
    }
    if($count==0) {return FALSE;}
    else {return TRUE;}
}
function sendmail($r,$to)
{
    $subject = "Your verification Code For HMS Hospital Registration";
    $code=(($r)%100000+($r)%10000+($r)%1000+($r)%100+($r)%10)%100000;
    $message = '
    <html>
    <head><title>Verification Code</title></head>
    <body>Dear sir,mam<br><br>
    Thanks for choosing HMS hospital, Please verify your account to activate follow this link <a href="http://hmshospital.netau.net/verification.php?verify='.$r.'">click here</a><br>
    Your verification Code is <b>'.$code.'.</b>  Must enter  to activate your account.
    </body>
    </html>';
    $headers = "MIME-Version: 1.0" . "\r\n";
    $headers .= "Content-type:text/html;charset=UTF-8" . "\r\n";
    $headers .= 'From: <hmshospitaliitkgp@gmail.com>' . "\r\n";
    mail($to,$subject,$message,$headers);
}
$msg="";
if(isset($_POST["submit"]))
{
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
$target_dir = "imguploads/";
$target_file = $target_dir . basename($_FILES["filetoupload"]["name"]);
$uploadOk = 2;
$imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
if(isset($_POST["submit"]))
{
    $check = getimagesize($_FILES["filetoupload"]["tmp_name"]);
    if($check !== false) $uploadOk = 1;
    else $uploadOk = 0;
}
if (file_exists($target_file)) $uploadOk=2;
if (($_FILES["filetoupload"]["size"] > 5000000)||($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg" && $imageFileType != "gif" ))
    $uploadOk = 0;
if($uploadOk == 1)
{
    if (move_uploaded_file($_FILES["filetoupload"]["tmp_name"], $target_file)) {$uploadOk=2;}
}
$address= $_POST['addr_line1'].' '.$_POST['addr_line2'];
if(isset($_POST["submit"]))
{
    $conn=mysqli_connect($host,$user,$pass,$db) or die('could not connect');
    $sqlquery = "Use ".$db.";";
    if ($conn->query($sqlquery) === FALSE) {echo "Error: " . $conn->error;die('unable to select database data');}
    $e_exist=email_exist($conn,$_POST["email"]);
    if($e_exist==FALSE && $uploadOk==2)
    {
        if($_POST['staff_key']=="")
        {
            $rand=gettimestamp();
            $sqlquery='insert into account values("'.$_POST['name'].'","'.$_POST['email'].'","'.$_POST['dob'].'","'.'patient'.'","'.'NULL'.'","'.$_POST['gender'].'","'.$address.'","'.$_POST['mob_no'].'","'.$_POST['s_password'].'","inactive","'.$rand.'");';
            if ($conn->query($sqlquery) === FALSE) {echo "Error: " . $conn->error;die('could not insert patient data');}
            rename($target_file,$target_dir.$_POST['email']);
            sendmail($rand,$_POST['email']);
            header('Location: verification.php?verify='.$rand);
        }
        else
        {
            $s_exist=staffkey_exist($conn,$_POST["email"],$_POST['staff_key']);
            if($s_exist==TRUE)
            {
                $rand=gettimestamp();
                $sqlquery='insert into account values("'.$_POST['name'].'","'.$_POST['email'].'","'.$_POST['dob'].'","'.'doctor'.'","'.$_POST['staff_key'].'","'.$_POST['gender'].'","'.$address.'","'.$_POST['mob_no'].'","'.$_POST['s_password'].'","inactive","'.$rand.'");';
                if ($conn->query($sqlquery) === FALSE) {echo "Error: " . $conn->error;die('could not insert patient data');}
                rename($target_file,$target_dir.$_POST['email']);
                sendmail($rand,$_POST['email']);
                header('Location: verification.php?verify='.$rand);
            }
            else $msg='You are Not valid staff Member';
        }
    }
    else if($e_exist==TRUE && $uploadOk==2) $msg='Email Already Exist';
    else if($e_exist==FALSE && $uploadOk!=2) $msg='InValid Image Try With Another Image';
    else if($e_exist==TRUE && $uploadOk!=2) $msg='Email Already Exist<br> InValid Image Try With Another Image';
}
}
?>

<div class="display_noti">
    <div class="image_panel" id="display_image">
        <form method="post" action="signup.php" name="signup" enctype="multipart/form-data" onsubmit="return validate_submit();">
            <div class="image_panel_top">
                <b>Create an Account</b>
            </div>
            <div class="image_panel_left">
                Name <br><br><br>
                E-mail<br><br><br>
                DOB <br><br><br>
                Gender (DD/MM/YYYY)<br><br><br>
                Staff Key<br><a style="font-size: 70%;">(Applicable to staff only)</a><br><br>
                Address line1<br><br><br>
                Address line2<br><br><br>
                Mobile No. <br><br><br>
                Password <br><br><br>
                Confirm Password<br><br><br>
                Upload Your Image<br><br><br>
            </div>
            <div class="image_panel_right">
                <?php
                if(isset($_POST["submit"]))
                {
                    echo '<input class="signup_top" type="text" name="name" id="name" length="" value="'.$_POST['name'].'" >&nbsp;<sup style="color:red;">*</sup></br><br>';
                    echo '<input class="signup_top" type="text" name="email" id="email" length="" value="'.$_POST['email'].'">&nbsp;<sup style="color:red;">*</sup></br><br>';
                    echo '<input class="signup_top" type="text" name="dob" id="dob" length="" value="'.$_POST['dob'].'">&nbsp;<sup style="color:red;">*</sup></br><br>';
                    echo '<input type="radio" id="gender1" name="gender" value="male" style="width: 10px;padding: 7px;height: 23px;"> Male';
                    echo '<input type="radio" id="gender2" name="gender" value="female" style="width: 10px;padding: 7px;height: 23px;"> Female';
                    echo '<input type="radio" id="gender3" name="gender" value="other" style="width: 10px;padding: 7px;height: 23px;"> Other';
                    echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
                    echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<sup style="color:red;">*</sup><br><br>';
                    echo '<input class="signup_bottom" type="text" name="staff_key" id="staff_key" length=""value="'.$_POST['staff_key'].'"><br><br>';
                    echo '<input class="signup_bottom" type="text" name="addr_line1" id="addr_line1" length="" value="'.$_POST['addr_line1'].'">&nbsp;<sup style="color:red;">*</sup><br><br>';
                    echo '<input class="signup_bottom" type="text" name="addr_line2" id="addr_line2" length="" value="'.$_POST['addr_line2'].'"></br><br>';
                    echo '<input class="signup_bottom" type="text" name="mob_no" id="mob_no" length="" value="'.$_POST['mob_no'].'">&nbsp;<sup style="color:red;">*</sup></br><br>';
                    echo '<input class="signup_bottom" type="password" name="s_password" id="s_password" length="" value="'.$_POST['s_password'].'">&nbsp;<sup style="color:red;">*</sup><br><br>';
                    echo '<input class="signup_bottom" type="password" name="c_password" id="c_password" length=""value="'.$_POST['c_password'].'">&nbsp;<sup style="color:red;">*</sup><br><br>';
                    echo '<input class="signup_bottom" type="file" name="filetoupload" id="filetoupload" value="'.$_POST['filetoupload'].'">&nbsp;<sup style="color:red;">*</sup>';
                }
                else
                {
                    echo '<input class="signup_top" type="text" name="name" id="name" length="" value="" >&nbsp;<sup style="color:red;">*</sup></br><br>';
                    echo '<input class="signup_top" type="text" name="email" id="email" length="" value="" >&nbsp;<sup style="color:red;">*</sup></br><br>';
                    echo '<input class="signup_top" type="text" name="dob" id="dob" length="" value="">&nbsp;<sup style="color:red;">*</sup></br><br>';
                    echo '<input type="radio" id="gender1" name="gender" value="male" style="width: 10px;padding: 7px;height: 23px;"> Male';
                    echo '<input type="radio" id="gender2" name="gender" value="female" style="width: 10px;padding: 7px;height: 23px;"> Female';
                    echo '<input type="radio" id="gender3" name="gender" value="other" style="width: 10px;padding: 7px;height: 23px;"> Other';
                    echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
                    echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<sup style="color:red;">*</sup><br><br>';
                    echo '<input class="signup_bottom" type="text" name="staff_key" id="staff_key" length=""value=""><br><br>';
                    echo '<input class="signup_bottom" type="text" name="addr_line1" id="addr_line1" length="" value="">&nbsp;<sup style="color:red;">*</sup><br><br>';
                    echo '<input class="signup_bottom" type="text" name="addr_line2" id="addr_line2" length="" value=""></br><br>';
                    echo '<input class="signup_bottom" type="text" name="mob_no" id="mob_no" length="" value="">&nbsp;<sup style="color:red;">*</sup></br><br>';
                    echo '<input class="signup_bottom" type="password" name="s_password" id="s_password" length="" value="">&nbsp;<sup style="color:red;">*</sup><br><br>';
                    echo '<input class="signup_bottom" type="password" name="c_password" id="c_password" length=""value="">&nbsp;<sup style="color:red;">*</sup><br><br>';
                    echo '<input class="signup_bottom" type="file" name="filetoupload" id="filetoupload" value="">&nbsp;<sup style="color:red;">*</sup>';
                }
                ?>
            </div>
            <div class="image_panel_bottom">
                <input name="submit" type="submit" value="Agree & Submit">
                 <?php echo '<h1 id="form_message" style="color: #DF0000; font-size: 25px;" >'.$msg.'</h1>'; $msg=""; ?>
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
        <li><a href="index.php" class="home"><b>Home</b></a></li>
        <li><a href="aboutus.php"><b>About Us</b></a></li>
        <li><a href="facilities.php"><b>Facilities</b></a></li>
        <li><a href="doctors.php"><b>Doctors</b></a></li>
        <li><a href="contactus.php"><b>Contact Us</b></a></li>
    </div>
    <div class="footer_centre">
        <b style="font-size: 200%;"><u>Partners</u></b>
        <li><a href="http://apolloari.com/" class="home"><b>Apollo Research & Innovations</b></a></li>
        <li><a href="http://www.aherf.org/"><b>Research Foundation</b></a></li>
        <li><a href="https://www.apollohospitals.com/research/apollo-medical-journal"><b>Apollo Medical Journal</b></a>
        </li>
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
                var myCenter = new google.maps.LatLng(22.31635, 87.29999);
                var mapCanvas = document.getElementById("map");
                var mapOptions = {center: myCenter, zoom: 16};
                var map = new google.maps.Map(mapCanvas, mapOptions);
                var marker = new google.maps.Marker({position: myCenter});
                marker.setMap(map);
                var infowindow = new google.maps.InfoWindow({
                    content: "HMS Hospital Kharagpur!"
                });
                infowindow.open(map, marker);
            }
        </script>
        <script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyBNbtr-tF7i4GnfMaeFkddQh70yGiGjoEI&callback=myMap"></script>
    </div>
</div>
</body>
</html>
