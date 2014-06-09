<?php
session_start();

//exec('/homes/cmd5414/public_html/cgi/clear.sh');
//MATCHMAKING
$pName=htmlspecialchars($_POST["pName"]);
$pPos=htmlspecialchars($_POST["pPos"]);
function cleanupQueue($file, $name){
      $data = file_get_contents($file, true);
      $patt = $name . ":" . $_SERVER['REMOTE_ADDR'] . ":" . $_SESSION["test"];
      $data = trim(preg_replace("/$patt/", "", $data)) . "\n";
      file_put_contents($file, $data);


}
function checkForPartner($file){
   if($file == "etc/queueP2"){
		
      $patt = $_SESSION['name'] . ":" . $_SERVER['REMOTE_ADDR'] . ":" . ".*";
      $data = file_get_contents($file) or die ("couldn't read the file \"$file\".");
      $dataLines = explode(PHP_EOL, $data);
      $matches = preg_grep("/^$patt$/", $dataLines);

      if(count($matches) > 0){
         $nameWithId = array_pop($matches);
         $parts = explode(":", $nameWithId);
         echo $nameWithId;
         $_SESSION["test"] = $parts[2];
         return ($nameWithId);
      }
   }
   elseif ($file == "etc/queueP1"){
      $opp="etc/queueP2";
      $patt = "abcd";
      $data = file_get_contents($opp) or die ("file not found \"$opp\".");
      $dataLines = explode(PHP_EOL, $data);
      $matches = preg_grep($patt, $dataLines);
      if(count($matches) > 0){
         $partner = array_pop($matches);
         $partArr = explode(":", $partner);
         $patt = "/$partner/";
         $replaceStr = $partner . ":" . $_SESSION['test'];
         $data = preg_replace($patt, $replaceStr, $data);
         file_put_contents($opp, $data);
         return ($partner);
      }
      else{
         return false;
      }

   }
   else{
      $opp = "Unknown File";
   }

}


function addToQueue($name, $f){
   $fHandle =fopen($f, "a") or die ("couldn't open the file");
   if($f == "etc/queueP1"){
      global $id;
      $id  = uniqid();
      $_SESSION["test"] = $id;
      fwrite($fHandle, $name . ":" . $_SERVER['REMOTE_ADDR'] . ":" . $id . "\n");
   }
   else{
      fwrite($fHandle, $name . ":" . $_SERVER['REMOTE_ADDR'] . "\n");
   }
   fclose($fHandle);
}

function alreadyInQueue($f){
   $patt = "/" . $_SERVER['REMOTE_ADDR'] . "/";
   $data = file_get_contents($f) or die ("couldn't read the file \"$f\".");

   if(preg_match($patt, $data)){

      return true;
   }
   else{

      return false;
   }
}


if(isset($_POST["qEntered"])){
   $qEntered=htmlspecialchars($_POST["qEntered"]);
}
else{
   $qEntered = "False";
}
$_SESSION['name'] = $pName;
$_SESSION['pos'] = $pPos;
if(isset($_SESSION['test'])){
   $id = $_SESSION['test'];
}
if (!isset($pName) || !isset($pPos)){
  exit("Must enter in a name and type" . $pName . $pPos);
}
if($pPos == "Player1"){
   $file="etc/queueP1";
}
elseif ($pPos == "Player2"){
   $file="etc/queueP2";
}
if($qEntered == "entered"){
   addToQueue($pName, $file);
}
else {
   $partner = checkForPartner($file);
   if(strlen($partner) > 0){
      cleanupQueue($file, $pName);

      if($pPos == "Player1"){
	  
	  
		}
         
      echo $partner;

   }
}



?>
