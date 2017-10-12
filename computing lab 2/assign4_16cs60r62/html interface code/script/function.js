/**
 * Created by batman on 21/3/17.
 */
function oninputevent(event) {
    var id=getsourceid(event);
    var textnode= document.getElementById(id).value;
    if(id=='login')
    {
        if(validateemail(textnode)==false)
        {

        }
    }
    if(id=='password')
    {
        if(textnode.length>5)
        {

        }
    }
}

function unikeycode(event) {
    var key = event.keyCode;
    //alert(key);
}
function getsourceid(e) {
    var source = e.target;
    return source.id;
}
function validateemail(str)
{
    var atposition=str.indexOf("@");
    var dotposition=str.lastIndexOf(".");
    if (atposition<1 || dotposition<atposition+2 || dotposition+1>=str.length){
        return false;
    }
    else if (str.length<5)return false;
    else return true;
}
function validatephonenumber(inputtxt) {
    var phoneno = /^\(?([0-9]{3})\)?[-. ]?([0-9]{3})[-. ]?([0-9]{4})$/;
    if(inputtxt.match(phoneno)) {
        return true;
    }
    else {
        return false;
    }
}
function validatestaffkey(inputtxt) {
    var staffk = /[A-Za-z]{4}[0-9]{6}]/;
    if(inputtxt.match(staffk)) {
        return true;
    }
    else {
        return false;
    }
}
function validatedobformat(inputtxt) {
    var tempdob = /[0-3][0-9][/][0-1][0-9][/][12][0-9]{3}/;
    if(inputtxt.match(tempdob)) {
        return true;
    }
    else {
        return false;
    }
}
var slideIndex = 1;
showSlides(slideIndex);

function plusSlides(n) {
    showSlides(slideIndex += n);
}

function currentSlide(n) {
    showSlides(slideIndex = n);
}

function showSlides(n) {
    var i;
    var slides = document.getElementsByClassName("mySlides");
    var dots = document.getElementsByClassName("dot");
    if (n > slides.length) {slideIndex = 1}
    if (n < 1) {slideIndex = slides.length}
    for (i = 0; i < slides.length; i++) {
        slides[i].style.display = "none";
    }
    for (i = 0; i < dots.length; i++) {
        dots[i].className = dots[i].className.replace(" active", "");
    }
    slides[slideIndex-1].style.display = "block";
    dots[slideIndex-1].className += " active";
}
function onloadfunction() {
}
function validate_submit(){
    var valid=true;
    var name=document.getElementById('name');
    var email=document.getElementById('email');
    var dob=document.getElementById('dob');
    var gender=null;
    if(document.getElementById('gender1').checked){gender=document.getElementById('gender1');gender.value="male";}
    if(document.getElementById('gender2').checked){gender=document.getElementById('gender2');gender.value="female";}
    if(document.getElementById('gender3').checked){gender=document.getElementById('gender3');gender.value="other";}
    var staff_key=document.getElementById('staff_key');
    var addr_line1=document.getElementById('addr_line1');
    var addr_line2=document.getElementById('addr_line2');
    var mob_no=document.getElementById('mob_no');
    var s_password=document.getElementById('s_password');
    var c_password=document.getElementById('c_password');
    var filetoupload=document.getElementById('filetoupload');
    if(name.value==''||email.value==''||dob.value==''|| gender==null||addr_line1.value==''||mob_no.value==''||
        s_password.value==''||c_password.value==''||filetoupload.value=='')
    {
        alert("* filed Can't Be Empty");
        valid=false;
    }
    else if (validateemail(email.value)==false)
    {
        email.style.border="2x solid #FF0000";
        valid=false;
    }
    else email.style.border = "2px solid black";
    if (validatedobformat(dob.value)==false)
    {
        dob.style.border="2x solid #FF0000";
        valid=false;
    }
    else dob.style.border = "2px solid black";
    if (validatestaffkey(staff_key.value)==false)
    {
        staff_key.style.border="2x solid #FF0000";
    }
    else staff_key.style.border = "2px solid black";
    if (validatephonenumber(mob_no.value)==false)
    {
        mob_no.style.border="2x solid #FF0000";
        valid=false;
    }
    else mob_no.style.border = "2px solid black";
    if(c_password.value.localeCompare(s_password.value)!=0)
    {
        s_password.style.border="2x solid #FF0000";
        c_password.style.border="2x solid #FF0000";
        valid=false;
    }
    else{
        s_password.style.border = "2px solid black";
        c_password.style.border = "2px solid black";
    }
    if(s_password.value.length<6)
    {
        s_password.style.border="2x solid #FF0000";
        valid=false;
    }
    else s_password.style.border = "2px solid black";
    if(c_password.value.length<6)
    {
        c_password.style.border="2x solid #FF0000";
        valid=false;
    }
    else c_password.style.border = "2px solid black";
    //var dataString = 'name='+name.value+'&email='+email.value+'&dob='+dob.value+'&gender='+gender.value+'&staff_key='+staff_key.value+
    //'&addr_line1='+addr_line1.value+'&addr_line2='+addr_line2.value+'&mob_no='+mob_no.value+'&password='+ s_password;
    //return valid;
    return valid;
}
