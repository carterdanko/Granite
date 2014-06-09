<html>
<head>
<script>

var http = createRequestObject();
var url = "http://granite.sru.edu/~cmd5414/cpsc317/final/matchmaking.php"

function matchFound(partner){
   window.clearInterval(timerInterval);
   var params = partner.split(":");
   var gameLoc = "http://granite.sru.edu/~cmd5414/cpsc317/final/simplewordgame.html"
   window.location = gameLoc;

}

function createRequestObject() {
    var ro;
    var browser = navigator.appName;
    if(browser == "Microsoft Internet Explorer"){
        ro = new ActiveXObject("Microsoft.XMLHTTP");
    }else{
        ro = new XMLHttpRequest();
    }
    return ro;
}
var timerInterval;

function startCounter(){
   var d = document.getElementById("timer");
   d.innerHTML="00:00";

   timerInterval = setInterval(updateTime, 1000);

}

function updateTime(){
   var d= document.getElementById("timer");

   count=d.innerHTML;

   var tArr=count.split(":");

   tArr[1] = "0" + (parseInt(tArr[1]) + 1);

   if(parseInt(tArr[1]) >= 60){
      tArr[1] = "00";
      tArr[0] = "0" + (parseInt(tArr[0])+1);
   }
   count=tArr[0].substring(tArr[0].length -2, tArr[0].length) + ":" + tArr[1].substring(tArr[1].length-2,tArr[1].length);
   d.innerHTML=count;
   if((parseInt(tArr[1]) % 3) == 0 ){
      http.onreadystatechange = function(){if (http.readyState==4 && http.status==200) if(http.responseText.length > 0) matchFound(http.responseText);};
      http.open('POST', url, true);
      http.setRequestHeader("Content-type","application/x-www-form-urlencoded");

      name=document.getElementById("nick").value;
      position=document.getElementById("role").value;
      http.send("pName=" + name+"&pPos="+position);
   }
}
function matchFound(partner){
   window.clearInterval(timerInterval);
   var gameLoc = "http://granite.sru.edu/~cmd5414/cpsc317/final/simplewordgame.html" 
   var params = partner.split(":");
   window.location = gameLoc;

}


function createRequestObject() {
    var ro;
    var browser = navigator.appName;
    if(browser == "Microsoft Internet Explorer"){
        ro = new ActiveXObject("Microsoft.XMLHTTP");
    }else{
        ro = new XMLHttpRequest();
    }
    return ro;
}


function enterQueue(name,position){
   http.onreadystatechange = function(){
        if (http.readyState==4 && http.status ==200) {
}

   };

   http.open('POST', url, true);
   http.setRequestHeader("Content-type","application/x-www-form-urlencoded");
   http.send("pName=" + name+"&pPos="+position+"&qEntered="+"entered");
   startCounter();

}

</script>

<body>

<h2 align = 'Center'> Simple Word Game </h2>
<h4 align = 'Center'> Carter Danko </h4>

<br><br>


<form name="queue" action="javascript:enterQueue(queue.role.value, queue.role.value)" align="center">
  Role:<br> <select name = "role" id = "role"> <option value="Player1">Player 1</option>
                               <option value="Player2">Player 2</option>
        </select>
  <br>
  <br>
  <input type="submit" value= "Get Match">
</form>

</body>

</html>

