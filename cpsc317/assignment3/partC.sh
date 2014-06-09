#!/bin/sh

letters=""
for i in "$@"
do
       letters="$letters$i"
done

letters=`echo $letters `


words="/usr/share/dict/words"

letterCount=`echo $name | wc -m` 
letterCount=`expr $letterCount - 1`

`echo rm -f ~/public_html/cpsc317/assignment3/assignment3temp`
name="assignment3temp"
fp=~/public_html/cpsc317/assignment3/$name
touch $fp


echo "<h2 align = 'center'> Name Table </h2> ">> $fp
table="<table border = '1' align = 'Center'>"
tablerows="<tr>"
tablerows="<td></td>"
for i in {1..6}
do
	#REMEMBER TO Cut lets so entire name doesnt show up and to try and make all letters capital
	letter=`echo $letters | cut -c$i | tr '[:lower:]' '[:upper:]'`
    tablerows+="<td width = '45' bgcolor = '#FF5050' >$letter</td>"
done
table+="$tablerows</tr>"
tablerows=""
for i in {1..6}
do
        for j in {0..6}
        do        
		if [ $j == 0 ]; then
                td=`echo $letters | cut -c$i | tr '[:lower:]' '[:upper:]'`
                tablerows+="<td width = '30' bgcolor = '#FF5050'>$td</td>"
		else
		    # REMEMBER TO Cut letters so the entire name doesnt show up
			begin=`echo $letters | cut -c$i`
			end=`echo $letters | cut -c$j`
			td=`grep -ic "^$begin.*$end$" $words`
	        tablerows+="<td width = '35'>$td</td>"
		fi
        done
        table+="$tablerows</tr>"
        tablerows=""
done
table+="</table>"
echo $table >> $fp
echo "</body></html>" >> $fp
mv $fp ~/public_html/table.html



