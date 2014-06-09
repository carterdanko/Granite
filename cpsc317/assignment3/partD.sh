#!/bin/sh

input1=$1
input2=$2
bothwords=""
str=`echo $input1 | grep -o "." | sort | uniq`
#load letters from word 1 into array withunique letters
for (( i=0; i<${#str}; i++ )); do
  word1Arr[$i]=${str:$i:1}
done

str=`echo $input2 | grep -o "." | sort | uniq`
#load letters from word 2 into array withuniqe letters
for (( i=0; i<${#str}; i++ )); do
  word2Arr[$i]=${str:$i:1}
done

#check to see if both words match and if they do increment bothwords with the letter that matches
for i in ${word1Arr[*]}
do
	for j in ${word2Arr[*]}
	do
		if [ "$i" == "$j" ]; then
			bothwords+="$j"
		fi
	done
done
echo "\"$input1\" and \"$input2\" share these ${#bothwords} "
echo -ne "\t"
for (( i = 0 ; i < ${#bothwords}; i++))
do
	if [ $i -eq `expr ${#bothwords} - 1` ]; then
		echo "\"${bothwords:$i:1}\"."
	else
		echo -ne "\"${bothwords:$i:1}\", "
	fi
done

