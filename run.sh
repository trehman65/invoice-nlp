#training and testing data generation
cd make	
./pricerightnlp
cd ..

#getting output
cd crfpp
./runtraintest.sh

cat output.txt