arr=( *.csv )


for i in "${arr[@]}" 
do 
	sed -i -e 's/QTY SHIPPING/QTY SHP/g' $i 
done