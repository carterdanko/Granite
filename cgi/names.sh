#! /bin/sh
echo "Content-type: text/html"
echo
echo
echo "<html><head><title>Name Return</title></head>"
echo "<body>"
stndinput=`tr -s '&' ' ' <&0`
name=`echo $stndinput | cut -d" " -f1 | cut -d"=" -f2`
sex=`echo $stndinput | cut -d" " -f2 | cut -d"=" -f2`
echo `~cmd5414/public_html/cpsc317/assignment4/partD.sh $sex $name`
echo "</body></html>"
 
