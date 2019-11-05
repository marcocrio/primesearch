// UT El Paso EE 4374 Assignment 3
// Multi-threaded Prime Number Search
// Author: Marco A Ramirez
//
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "mramirez_prime.h"

/* Global variables that are shared */
sPRIME_THREAD primeThreadData[MAX_THREADS];
int	numThreads;

void *prime_search(void *param)
{
	sPRIME_THREAD* parameters= (sPRIME_THREAD *)param;//normilizes parameter (typecasts to use)
	

	unsigned long i;
	/* prepare*/
	FILE *primesfile;//defines name for files to open
	char filesname[10];
	sprintf(filesname,"primes%d",parameters->num);// creates file name string
	
	primesfile = fopen(filesname,"w");// opens files
	
    parameters->current = parameters->low; // sets the current as the lower limit

	/*Checks if numbers are prime and writes them into file*/
	for(; parameters->current < parameters->high; parameters->current++){
		if(test_prime(parameters->current)){
			fprintf(primesfile, "%u\n", parameters->current);
		}
	}
    
	fclose(primesfile);
	

	pthread_exit(0);
	
}

void *mini_shell(void *param)
{
	int inputSize, i;
	int threadNum;
	char buff[128];		// Character buffer

	while(1)
	{
		// Request a line of input
		write(1, " -> ", 4);
		// Read the line
		inputSize = read(0, buff, 128);
		// Strip off carriage return
		buff[inputSize-1] = '\0';
		
		if((buff[0] >= '1') && (buff[0] <= '9'))
		{
			// An integer was entered
			threadNum = buff[0] - '0';
			if(threadNum <= numThreads)
			{
				printf("Thread %d progress: %d\n", threadNum, primeThreadData[threadNum-1].current);
			}
			else
			{
				printf("Thread %d does not exist\n", threadNum);
			}
			
		}
		else
		{		
			if(buff[0] == 'a')
			{
				// Print status for all threads
				for(i = 0; i < numThreads; i++)
				{
					printf("Thread %d progress: %d\n", i+1, primeThreadData[i].current);
				}
			}
			else
			{
				printf("Unrecognized input\n");
			}
		}

		printf("\n");
		fflush(NULL);

	}

	pthread_exit(0);

}

int test_prime(int n)
{	

	if(n==1 || n==0) return 0; // 1 and 0 are not considered primes

	if(n < 1) return 0; //negative numbers arent consider primes
	int i, flag=0;
	//checks for each number to see if it is prime
	for(i = 2; i <= n/2; ++i)
    {
        // condition for nonprime number
        if(n%i == 0)
        {
            flag = 1;//flags if not prime
            break;
        }
    }
	
	if(!flag) return 1;//returns 1 if prime
	else return 0;
}
