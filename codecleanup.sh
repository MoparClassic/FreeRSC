cd ./include
astyle --style=k/r --mode=c -S -w -m0 -M40 -p -y -o *.h
rm -rf *.orig
cd ..

cd ./src
astyle --style=k/r --mode=c -S -w -m0 -M40 -p -y -o *.c
rm -rf *.orig
cd ..

cd ./tests
astyle --style=k/r --mode=c -S -w -m0 -M40 -p -y -o *.c
rm -rf *.orig
cd ..