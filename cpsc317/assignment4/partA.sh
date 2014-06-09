#!/bin/sh

lettercount="^"
wordlength=$1
words=/usr/share/dict/words
if [[ -n ${wordlength//[0-9]/} ]];then
  echo "Not an integer!"
  exit 2
else
  for((i=0;i<$wordlength;i++))
   do
  lettercount+="."
done
fi
lettercount+="$"

num=`grep -c "\($lettercount\)" $words`
randnumber=$RANDOM
randnumber=$((randnumber%$num))
word=`grep "$lettercount" $words | head -$randnumber | tail -1`

echo "There are $num $wordlength-letter words, including \"$word\""
