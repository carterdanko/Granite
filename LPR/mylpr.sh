#!/bin/sh

#THERE ARE 63 LINES PER PAGE SO MAKE SURE 
filename=$1
#/homes/cmd5414/cpsc207/LPR/asciiart
location=`pwd`/$1
echo "Username: "$USER>>file$$.txt
echo "Process ID: "$$>>file$$.txt
echo "Filename: "$1>>file$$.txt
cat /homes/cmd5414/cpsc207/LPR/asciiart >>file$$.txt
#cat $location
for i in {1..31}
do
echo " ">>file$$.txt
done
cat $location>>file$$.txt
lp -dAT224 file$$.txt
