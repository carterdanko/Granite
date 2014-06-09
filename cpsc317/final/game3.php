<?
$file="../gamelog";
$file="/homes/cmd5414/public_html/cpsc317/final/gameoutputtest";
$wordlist="/homes/cmd5414/public_html/cpsc317/final/dictwords";
exec('/homes/cmd5414/public_html/cgi/finduniqs.sh');
$w=fopen($wordlist, 'r');
$o=file($file);
$last=$o[count($o)-1];
$words=explode(" ",$last);
$first=$words[0];
$data=$_GET["data"];
$id=$_GET["id"];
$role=$words[2];
//$role=%_SESSION[test];

//The goal was to see that whatever the user put, whether it be Player1 or Player2
//be used as input to then determine what file it writes to.  Could not get proper value
//so tried to use the last element from the array words, but nothing still.
//Right now the only output from the game is to player1 in etc folder of the game.

//if($role=="Player1"){
$player1count=0;
$player1correct="/homes/cmd5414/public_html/cpsc317/final/gamedata/player1correct.txt";
$pc=fopen($player1correct,'a+');
$player1guess="/homes/cmd5414/public_html/cpsc317/final/gamedata/player1guess.txt";
$pg=fopen($player1guess,'a+');
//}


if($role=="Player2"){
//echo "TESTING THIS IS A TEST";
//$player2count=0;
//$player2correct="/homes/cmd5414/public_html/cpsc317/final/gamedata/player2correct.txt";
//$pc2=fopen($player2correct,'a+');
//$player2guess="/homes/cmd5414/public_html/cpsc317/final/gamedata/player2guess.txt";
//$pg2=fopen($player2guess,'a+');
}

$linecountcorrect=0;
$linecountguess=0;
$lcgt=0;
while(!feof($pc)){
 $linecorrect = fgets($pc);
 $linecountcorrect++;
 }
while(!feof($pg)){
 $lineguess = fgets($pg);
 $lcgt++;
 $linecountguess=$lcgt-1;
 }

//echo $linecount;
if (!isset($data)) $data="hello";
if (!isset($id)) $id="0000000000";

if ($linecountguess>"16"){
echo "You have used all of your guesses";
echo "<br>You got ".$linecountcorrect." correct";
exit();
}
if ($linecountcorrect=="10"){
echo "You got all 10 correct!";
exit();
}

if ($w) {
   $array = explode("\n", fread($w, filesize($wordlist)));
		//print_r(array_values($array));
		foreach($array as $item){
		$strlength=strlen($item);
		$firstletter=substr($item, 0, -($strlength-1));
		echo $firstletter;
		for($i=0;$i<$strlength-2;$i++)
		{
			echo " _ ";
		}
		$lastletter=substr($item,-1);
		echo $lastletter;
		//echo $item;
		echo "<br>";
		}
		
		
}

if( strpos(file_get_contents($wordlist),$data) !== false || strpos(file_get_contents($player1correct),$data) !== false) {
        echo "<br><br>";
		echo "You are correct in guessing: ";
		echo $data;
		//echo "<br><br>";
		$player1count++;
		//echo $player1count;
		echo "<br><br>";
		fwrite($pc,$data."\n");
		fwrite($pg,$data."\n");
   		fclose($pc);
		fclose($pg);
		echo "You have: ".$linecountcorrect." correct";
		echo "<br>";
		echo "You have guessed a total of: ".$linecountguess." times";
	 }
	
if( strpos(file_get_contents($wordlist),$data) == false) {
        
		//echo $data;
		//echo " is not a correct answer";
		
		//echo $player1count;
		echo "<br><br>";
		fwrite($pg,$data."\n");
                echo "You have guessed a total of: ".$linecountguess." times";
				//echo $words[2];
    		fclose($pc);
                fclose($pg);

	}

else{

	$r=$_SERVER['REMOTE_ADDR'];

	$idstring=$id." ".$r;

	$o=fopen($file, "a") or die ("couldn't open $file");
	fwrite($o,$idstring." ".$data."\n");
	//fclose($o);
}
?>
