#!/bin/sh

rm -rf perms123.txt
touch perms123.txt

for alg in "l" "p" "c" "e"
do
  echo ""
  echo "Testing algorithm ${alg}"
  ./permgen.exe -${alg} -n 3 -o perms.txt
  echo "Algorithm ${alg}" >> perms123.txt
  cat perms.txt >> perms123.txt
  echo "" >> perms123.txt
done

if [ "$SECONDS" -eq 1 ]
then
  units=second
else
  units=seconds
fi
echo "This script has been running $SECONDS $units."

echo "Ran all tests."
