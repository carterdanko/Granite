#!/bin/sh
#Did get help from online searching sed with substitute, nothing was copied though, just research from examples online.
postal=postal.html
input1=$1
output1=`grep [A-Z][A-Z] $postal | sed -e 's/<td>//g' -e 's/<\/td>//g' | grep -i  "$input1 "`
state=`echo $output1 | cut -d " " -f3`
echo $state

