#!/bin/sh
uniq player1guess.txt > removedoubles.txt
cat removedoubles.txt > player1guess.txt
uniq player1correct.txt > removedoubles.txt
cat removedoubles.txt > player1correct.txt

uniq player2guess.txt > removedoubles.txt
cat removedoubles.txt > player2guess.txt
uniq player2correct.txt > removedoubles.txt
cat removedoubles.txt > player2correct.txt





