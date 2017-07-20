cd /Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/inputdir

files=( $(ls *.txt) )

cd /Users/talha/Coding/ClionProjects/pricerightnlp/crfpp

for i in ${files[@]}
do

echo "Testing: "$i
/Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/crf_test -v1 -m /Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/pricerightcol /Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/inputcol/$i > /Users/talha/Coding/ClionProjects/pricerightnlp/crfpp/outputcol/$i

done
