<html>
<style>
span.p{}
</style>
<script>
function place(p){
        u="http://granite.sru.edu/~cmd5414/cpsc317/assignment7/test.php?"
        window.location=u+"place="+p

}

function code(p){
	u="http://granite.sru.edu/~cmd5414/cpsc317/assignment7/test.php?"
	window.location=u+"state="+p
}

</script>
<body><table border=1>
<?php


$ss=$_POST[$state];

$f="/homes/cmd5414/public_html/cpsc317/assignment7/fixed.txt";
$o=file($f);


if (isset($ss)){
	echo "alert('test')";
}


elseif (isset($place)){
	
}
else{


for ($i=0;$i<8;$i++){
        $p= $o[rand(0,count($o)+1)];
        $p=trim($p);
        $parts=explode(" ",$p);

        echo "<tr><td><a href='#' onclick=place('".$parts[1]."')>";
	for ($k=0;$k<10;$k++)
	{
		if (isset($parts[$k+1])==true)
		{
			echo $parts[$k+1];
			echo " ";
		}
	}
        echo "</a>";
        echo "</td><td><a href='#' onclick=code('".$parts[0]."')>";
        $SC=$parts[0];
	$code=$parts[0];
        echo $SC;
        $state=`/homes/ddailey/lookup $SC`;
        echo "<br>".$state;
        echo "</a></td></tr>";
}
}


?>
</table></body></html>

