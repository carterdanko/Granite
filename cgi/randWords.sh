#/bin/sh

file="threefull"
sort -R $file | head -18 | awk '{print $1,$3,$4;}'
