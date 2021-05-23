finally: program 
	./program

program: mapper.o main.o
	gcc -o program mapper.o main.o

mapper.o: mapper.c
	gcc -c mapper.c

main.o: main.c
	gcc -c main.c

clean:
	rm program mapper.o main.o

chunks:
	rm chunk1.txt chunk2.txt chunk3.txt chunk4.txt