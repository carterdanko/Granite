<html>
<head>
<script>

function redirect(word, freq, type){
   var url="http://granite.sru.edu/~cmd5414/cpsc317/assignment6/partB.php";
   url+="?sWord=" + word +  "&freq=" + freq + "&type=" + type;
   window.location= url;   
}
</script>
</head>
<body>
<?php
function words()
{
   exec('~cmd5414/public_html/cgi/randWords.sh', $wordArray);
   for($i = 0; $i < count($wordArray); $i++){
      $wordArray[$i] = explode(" ", $wordArray[$i]);
   }
   return $wordArray;
}
?>
<h3 align = "Center"> Assignment 6 Part B</h3>
<?php
$word = $_GET["sWord"];
$freq=$_GET["freq"];
$type=$_GET["type"];
if (isset($word) && isset($freq) && isset($type)){  
   echo "<center>\"$word\" is a $type and has a frequency of $freq.</center>";
   echo "<br>";
}
$rowNum = 6;
$colNum = 3;
$wordList= words();
   $table = "<table border  = '1' align = 'Center'>";
   for($i = 0; $i < $rowNum; $i++){
      $tr = "<tr>";
      for($j = 0; $j < $colNum; $j++){
	 $wordInfo=$wordList[(($i * $colNum) + $j)];
	 $randWord = $wordInfo[1];
	 $td = "<td onclick = \"redirect('" . $wordInfo[1] . "','" . $wordInfo[0] . "','" . $wordInfo[2] . "')\" " . ">" . $randWord . "</td>";     
	 $tr = $tr . $td;
      }
      $tr = $tr . "</tr>";
      $table = $table . $tr;
   }
   $table = $table . "</table>";
   echo $table;
?>
</body>
</html>
