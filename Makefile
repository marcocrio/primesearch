# Complete this rule
mramirez_prog3: mramirez_prime.o mramirez_prog3.c
	gcc -g -pthread mramirez_prime.c mramirez_prog3.c -o mramirez_prog3

# Replace with correct file names
mramirez_prime.o: mramirez_prime.c mramirez_prime.h
	gcc -c mramirez_prime.c

clean:
	rm *.o; rm mramirez_prog3 primes1 primes2 primest

