<?php
session_start();
?>
<!DOCTYPE html>
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
$sqlquery='select * from account where email="'.$_POST['username'].'" and password="'.$_POST['password'].'" and state="active";';
$result=$conn->query($sqlquery);
$name="";
$email="";
$rand="";
$type="";
if ($result->num_rows==1)
    {
        while($row = mysqli_fetch_row($result))
        {
            $name=$row[0];
            $email=$row[1];
            $type=$row[3];
            $rand=$row[10];
        }
    $rand=gettimestamp();
    //$sqlquery='UPDATE account SET temp="'.$rand.'" WHERE email="'.$_POST['username'].'";';
    //$result=$conn->query($sqlquery);
    $_SESSION["email"] =$email;
    $_SESSION["name"] = $name;
    $_SESSION["rand"] = $rand;
    $_SESSION["type"] = $type;
    echo $rand;
    echo $sqlquery;
    header('Location: account.php');
    }
else
{
    session_unset();
    session_destroy();
    header('Location: index.php');
}
?>