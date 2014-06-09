#! /bin/sh
echo "Content-type: text/html"
echo
echo
echo "<html><head><title>Population name lookup</title></head>"
echo "<body>"
gender="/homes/cmd5414/public_html/cpsc317/assignment4"
elinks -source "http://www.census.gov/population/www/popclockus.html" > data
daileydata="data"

malefrequency=.482
femalefrequency=.518

male=$gender/dist.male.first
female=$gender/dist.female.first
lastName=$gender/dist_all_last

firstname=$1
lastname=$2
malepercent=`grep -iw $firstname $male | tr -s ' ' ':' | cut -d":" -f2`
femalepercent=`grep -iw $firstname $female | tr -s ' ' ':' | cut -d":" -f2`
lastpercent=`grep -iw $lastname $lastName | tr -s ' ' ':' | cut -d":" -f2`
totalpercent=`echo "$malepercent * $malefrequency + $femalepercent * $femalefrequency" | bc`
currpopulation=`grep 'div .*usnumber' $daileydata | cut -c 12- | cut -d">" -f2 | cut -d"<" -f1`
date=`date`
comppop=`grep 'div .*usnumber' $daileydata | cut -c 12- | cut -d">" -f2 | cut -d"<" -f1 | sed 's/,//g'`
lastpop=`echo "scale=8;(($lastpercent / 100 ) * $comppop)" | bc | cut -d"." -f1`
actual=`echo "scale=4;$totalpercent / 100" | bc`
relativepop=`echo "$comppop * $actual" | bc | cut -d"." -f1`
both=`echo "scale=4; (($actual) * $comppop * $lastpercent )" | bc`
both=`printf "%'.f" $combo`
echo "Approximately " $malepercent "% of males and " $femalepercent "% of females are named $firstname."
echo "<br><br>"
echo "Hence, about " $totalpercent "% of Americans are named $firstname"
echo "<br><br>"
echo "There are currently " $currpopulation " Americans (estimated by US Census's population clock on " $date " )"
echo "<br><br>"
echo "Therefore there are " $relativepop " Americans with the name $firstname "
echo "<br><br>"
echo "The last name " $lastname " occurs roughly " $lastpercent " which means there are about " $lastpop $lastname"s in America"
echo "<br><br>"
echo "Overall, we'd expect that we have " $both " people named \"$firstname $lastname\""

echo "</body></html>"






