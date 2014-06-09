#!/bin/sh

city=""
places=places.txt
for i in "$@"
do 
  city="$city $i"
done
spacing=`grep -i "$city" $places`
state=`echo $spacing | grep -ow '[A-Z][A-Z]'`
idontknowname=`echo $state`
for i in $idontknowname
do
  state=`./partA.sh $i`
  echo $state
done


