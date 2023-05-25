CC=gcc -std=c11
CFLAGS=-c


all: clean A B

A: A.o  
	$(CC) A.o -o prog

B: B.o 
	$(CC) B.o -o prog

A.o:
	$(CC) $(CFLAGS) A.c

B.o:
	$(CC) $(CFLAGS) B.c

clean:
	rm -rf *.o *.exe main *.a tests/*.o test *.gcov *.gcda *.gcno *.info report
