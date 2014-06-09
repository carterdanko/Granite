#! /bin/sh

if [ "$1" == "female" ]; then
	input1="/homes/cmd5414/public_html/cpsc317/assignment4/dist.female.first"
elif [ "$1" == "male" ]; then
	input1="/homes/cmd5414/public_html/cpsc317/assignment4/dist.male.first"
elif [ "$1" == "all" -o "$1" == "last" ]; then
	input1="/homes/cmd5414/public_html/cpsc317/assignment4/dist_all_last"
else
	echo "Error: You must input correct parameters"
	exit 2
fi

name=""
name="^"
name+=`echo $2 | tr '[:lower:]' '[:upper:]'`
numberOfNames=0
numberOfNames=`grep -c "\($name\)" $input1`
nameoutput=`echo $name | sed -r s/^.{1}//`
echo "There are $numberOfNames names that start with $nameoutput"
if [ $numberOfNames -eq 0 ]; then
	exit 0
else
	if [ $numberOfNames -gt 3 ]; then
		numberOfNames=3
	fi
	echo "The top $numberOfNames names in the list which start with letter $nameoutput are:"
fi
grep "$name" $input1 | head -3 | tr -s ' ' ' ' | awk -F' ' '{print "Name: ", $1, "\tRank:", $4}'
echo
