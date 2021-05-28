finally: program 
	./program

program: mapper.o main.o reducer.o
	gcc -o program mapper.o main.o reducer.o

reducer.o: reducer.c
	gcc -c reducer.c

mapper.o: mapper.c
	gcc -c mapper.c

main.o: main.c
	gcc -c main.c

clean:
	rm program mapper.o main.o reducer.o a.out

chunks:
	rm chunk1.txt chunk2.txt chunk3.txt chunk4.txt

archivos:
	rm A11 A12 A21 A22 A13 A23 A14 A24