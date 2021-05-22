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