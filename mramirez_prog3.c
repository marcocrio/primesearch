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
#include <stdlib.h>
#include "mramirez_prime.h"

#define BUFFER_SIZE	1000000

unsigned char buffer[BUFFER_SIZE+1];
unsigned char fileName[100];
FILE *primeFile;
FILE *primeThreadFile;


int main(int argc, char *argv[])
{

  if(argc < 2)
    {
        printf("Please provide an argument with the range to find prime numbers\n");
        return -1;
    }

  int i, bytesRead, bytesWritten;
  pthread_t tid[MAX_THREADS]; 
  pthread_t tidshell;
  pthread_attr_t attr;
  time_t before, after;

  /* Record time at start */
  before = time(NULL);

  /* Setup threads to find prime numbers */
  pthread_attr_init(&attr);
  numThreads = 2;
  


  // sets up threads
  long normarg = (long)atol(argv[1]); // normalizes argument

  /*To balance the load an summation (integral) of the digits on the range was taken
    to find where the area under the curve was exactly half thus balancing the number
    of iterations that each of the 2 threads had to go over.
    the ratio turned out to be 70% had to be the lower range and 30% the higher range to
    have a blanced load*/
  long rangeThreadOne = (normarg*0.7);//gets lower range
  long rangeThreadTwo = (normarg) - rangeThreadOne;// gets higher range
 
  //initializes values for thread 0
  primeThreadData[0].num = 1;
  primeThreadData[0].low = 1;
  primeThreadData[0].high = rangeThreadOne;

  //initializes values for threae 1
  primeThreadData[1].num = 2;
  primeThreadData[1].low = rangeThreadOne+1;
  primeThreadData[1].high = normarg;

  //creates both tr
  pthread_create(&tid[0],&attr,prime_search,(void *)&primeThreadData[0]);
  pthread_create(&tid[1],&attr,prime_search,(void *)&primeThreadData[1]);


  /* Setup a mini shell thread to provide interactivity with the user */
  pthread_create(&tidshell,&attr,mini_shell,NULL);




  


  /* Create primes output file */
  primeFile = fopen("primest","w");
  fclose(primeFile);

  /* Wait for the prime search threads to complete and combine their data */
  for(i = 0; i < numThreads; i++)
  {
  	/* Wait for the next thread to complete */
  	pthread_join(tid[i],NULL);
  	/* On thread completion, write its data to "primest" */
    fileName[0] = '\0';
    sprintf(fileName, "primes%d", i+1);					// Open the thread's data file
    if((primeThreadFile = fopen(fileName,"r")) == NULL)
    {
    	printf("Failed to open file: %s\n", fileName);
    }
    else
    {
    	if((primeFile = fopen("primest","a")) == NULL)	// Open "primest"
    	{
    		printf("Failed to open file: primest\n");
    	}
    	else
    	{
    		while(feof(primeThreadFile) == 0)
			{
				/* Read from the thread's data file */
				bytesRead = fread(buffer,1,BUFFER_SIZE,primeThreadFile);
				/* Write to the combined file */
				bytesWritten = fwrite(buffer,1,bytesRead,primeFile);
    		}
			fclose(primeFile);
    	}
		fclose(primeThreadFile);
	}
  }
  
  /* Record execution time */
  after = time(NULL);
  printf("\nPrime search done after %ld seconds\n", after-before);




  sleep(20);
  
  /* Lastly, kill the interaction thread */
  pthread_kill(tidshell, SIGKILL);

  return 0;

}

