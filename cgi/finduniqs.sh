#!/bin/sh
uniq /homes/cmd5414/public_html/cpsc317/final/gamedata/player1guess.txt > /homes/cmd5414/public_html/cpsc317/final/gamedata/removedoublesp1g.txt
cat /homes/cmd5414/public_html/cpsc317/final/gamedata/removedoublesp1g.txt > /homes/cmd5414/public_html/cpsc317/final/gamedata/player1guess.txt
uniq /homes/cmd5414/public_html/cpsc317/final/gamedata/player1correct.txt > /homes/cmd5414/public_html/cpsc317/final/gamedata/removedoublesp1c.txt
cat /homes/cmd5414/public_html/cpsc317/final/gamedata/removedoublesp1c.txt > /homes/cmd5414/public_html/cpsc317/final/gamedata/player1correct.txt

#uniq /homes/cmd5414/public_html/cpsc317/final/gamedata/player2guess.txt > /homes/cmd5414/public_html/cpsc317/final/gamedata/removedoubles.txt
#cat /homes/cmd5414/public_html/cpsc317/final/gamedata/removedoubles.txt > /homes/cmd5414/public_html/cpsc317/final/gamedata/player2guess.txt
#uniq /homes/cmd5414/public_html/cpsc317/final/gamedata/player2correct.txt > /homes/cmd5414/public_html/cpsc317/final/gamedata/removedoubles.txt
#cat /homes/cmd5414/public_html/cpsc317/final/gamedata/removedoubles.txt > /homes/cmd5414/public_html/cpsc317/final/gamedata/player2correct.txt





