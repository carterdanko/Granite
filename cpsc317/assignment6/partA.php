<html>
<body>

<form name="f" method="GET" action="http://granite.sru.edu/~cmd5414/cpsc317/assignment6/partA.php">
<select name="sBegin">
<option value="A" name="A">A</option>
<option value="B" name="B">B</option>
<option value="C" name="C">C</option>
<option value="D" name="D">D</option>
<option value="E" name="E">E</option>
<option value="F" name="F">F</option>
<option value="G" name="G">G</option>
<option value="H" name="H">H</option>
<option value="I" name="I">I</option>
<option value="J" name="J">J</option>
<option value="K" name="K">K</option>
<option value="L" name="L">L</option>
<option value="M" name="M">M</option>
<option value="N" name="N">N</option>
<option value="O" name="O">O</option>
<option value="P" name="P">P</option>
<option value="Q" name="Q">Q</option>
<option value="R" name="R">R</option>
<option value="S" name="S">S</option>
<option value="T" name="T">T</option>
<option value="U" name="U">U</option>
<option value="V" name="V">V</option>
<option value="W" name="W">W</option>
<option value="X" name="X">X</option>
<option value="Y" name="Y">Y</option>
<option value="Z" name="Z">Z</option>
</select>
<select name="sEnd">
<option value="A" name="A">A</option>
<option value="B" name="B">B</option>
<option value="C" name="C">C</option>
<option value="D" name="D">D</option>
<option value="E" name="E">E</option>
<option value="F" name="F">F</option>
<option value="G" name="G">G</option>
<option value="H" name="H">H</option>
<option value="I" name="I">I</option>
<option value="J" name="J">J</option>
<option value="K" name="K">K</option>
<option value="L" name="L">L</option>
<option value="M" name="M">M</option>
<option value="N" name="N">N</option>
<option value="O" name="O">O</option>
<option value="P" name="P">P</option>
<option value="Q" name="Q">Q</option>
<option value="R" name="R">R</option>
<option value="S" name="S">S</option>
<option value="T" name="T">T</option>
<option value="U" name="U">U</option>
<option value="V" name="V">V</option>
<option value="W" name="W">W</option>
<option value="X" name="X">X</option>
<option value="Y" name="Y">Y</option>
<option value="Z" name="Z">Z</option>
</select>
<input type="submit">
<br>
<?php
function stringGen($first, $last)
{
   $variable1idk = 0;
   $variable2idk = "";
   for ($i=0; $i < 3; $i++){
      $variable1idk = rand(97,122);
      $variable2idk = $variable2idk . chr($variable1idk);
   }
   $combined = $first . $variable2idk . $last; 
   return $combined;
}
$sBegin=$_GET["sBegin"];
$sEnd=$_GET["sEnd"];
if (isset($sBegin) && isset($sEnd)){  
   for ($i=0; $i < 3; $i++){
      print(stringGen($sBegin,$sEnd));
      echo "<br>";
   }
} 
?>
</body>
</html>













