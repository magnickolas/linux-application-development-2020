set -e
FILENAME1=test
FILENAME2=testFIX
touch $FILENAME1
touch $FILENAME2
[[ -f $FILENAME1 && -f $FILENAME2 ]]

rm $FILENAME1
rm $FILENAME2

[[ ! -f $FILENAME1 && ! -f $FILENAME2 ]]
