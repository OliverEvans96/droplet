#1st arg - input file
#2nd arg - output file
echo "Isolating substrate"
mawk '{ if (!(NF==8 && $2>=4)) print > "'"$2"'" ;if (NR%50000000==0) print "Parsed line "NR" of '"$1"'";}' $1
echo "Done isolating substrate!"
