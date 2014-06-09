#! /bin/sh
echo "Content-type: text/html"
echo
echo
echo "<html><head><title>Submit Test </title></head>"
echo "<body>"
input=""
if [ "$REQUEST_METHOD" == "GET" ]; then
	if [ "$#" -gt "0" ]; then
		echo The number of input parameters is $#"<br><br>"
		echo The first $# parameters are:"<br>"
		echo "<blockquote>"$1 $2	
		i=1
		for var in "$@"
		do
			input+="#$i=$var "
			i=`expr $i + 1`
		done		
		numParams=$#
		echo "</blockquote>"
	else
		echo "The method used was GET"
		echo "<br> \$QUERY_STRING is:"
		echo "<blockquote>"$QUERY_STRING
		echo "</blockquote>"
		input=`echo $QUERY_STRING | tr -s '&' ' '` 
		numParams=`echo $input | wc -w`
		echo "</blockquote>"
	fi
else 
	echo content length is $CONTENT_LENGTH "<br>"
	echo STDIN contains:"<blockquote>"
	input=`cat <&0`
	echo $input "</blockquote>"
	input=`echo $input | tr -s '&' ' '`
	numParams=`echo $input | wc -w`
fi

	echo The $numParams fields: 
	echo "<blockquote>"
	for((i=1; i <= $numParams; i++)) ;
	do
		parameterSomething=`echo $input | cut -d" " -f$i`
		name=`echo $parameterSomething | cut -d"=" -f1`
		value=`echo $parameterSomething | cut -d"=" -f2`
		value=`echo $value | sed 's/%/\\\x/g'`
		echo "<li>" $name=$value "</li>"
	done
	echo "</blockquote>"
echo "</body></html>"
