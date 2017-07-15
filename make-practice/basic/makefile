main: ./src/main.o ./src/alt.o 
	cc -o ./build/main ./src/main.o ./src/alt.o

./src/main.o: ./src/main.c ./src/include/alt.h
	cc -c ./src/main.c -o ./src/main.o

./src/alt.o: ./src/alt.c ./src/include/alt.h
	cc -c ./src/alt.c -o ./src/alt.o

clean:
	rm ./src/*.o


