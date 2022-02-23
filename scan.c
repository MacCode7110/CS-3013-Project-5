#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <math.h>
#define MAX_LINE_SIZE 256

//Hillis Steele Approach with a barrier created by condition variables

//Globals:

int *input; //shared resource among all threads
int n = 0; //shared resource among all threads
int numthreads = 0; //shared resource among all threads
//pthread_mutex_t lock; //Declare the mutex for locking purposes

//Global Condition Variables:

int moveontonextcomputationlevel = 1; //Used to tell a thread when to move onto the next level/iteration of sums/computations
int prefixsumparallelcalccomplete = 0; //Used to tell a thread when the prefix sum calculation is complete
double numsteps = 0; //Used to keep trakc of the number of individual sums

//Global Hillis Steele Algorithm Variable:
int addacrossinterval = 1;

//Read in input from the argument file into an array of integer pointers
void read_input_vector(const char* filename, int n, int* array)
{
  FILE *fp;
  char *line = malloc(MAX_LINE_SIZE+1);
  size_t len = MAX_LINE_SIZE;
  ssize_t read;

  fp = strcmp(filename, "-") ? fopen(filename, "r") : stdin;

  assert(fp != NULL && line != NULL);

  int index = 0;

  while((read = getline(&line, &len, fp)) != -1)
  {
	array[index] = atoi(line);
	index++;
  }

  free(line);
  fclose(fp);
}

//Calculate the prefix sum, which contains the critical section
void* calculateprefixsum()
{
	//Provide a lock to the current thread that has entered the critical section so that no other locks can enter at the same time
	//pthread_mutex_lock(&lock);

	//Critical Section

	if(prefixsumparallelcalccomplete == 0)
	{
		if(moveontonextcomputationlevel == 1)
		{
			moveontonextcomputationlevel = 0;

			for(int i = 0; i < sizeof(input); i++)
			{
				if((i + addacrossinterval) <= (sizeof(input) - 1))
				{
					input[i + addacrossinterval] = input[i] + input[i + addacrossinterval];
					numsteps++;

					//NOTE: All input[i] proceeding the first input[i + addacrossinterval] will retain the same value.

					//Determine if the prefix sum calculation has finished by seeing if the number of steps = log2(number of numbers to sum)
					if(numsteps == log2(sizeof(input)))
					{
						prefixsumparallelcalccomplete = 1;
					}
				}
			}

			//Update variables for next level of computation
			addacrossinterval = addacrossinterval * 2;
			moveontonextcomputationlevel = 1;
		}
	}

	//Release the lock so that other threads now have a chance to enter the critical section:
	//pthread_mutex_unlock(&lock);

	return NULL;
}

//Print the prefix sum
void* printPrefixSum()
{
	for(int i = 0; i < n; i++)
	{
		printf("%d\n", input[i]);
	}
	return NULL;
}

int main(int argc, char* argv[])
{
  //argv[0] is a pointer to the name of the program being run.
  char* filename = argv[1];//filename pointer
  n = atoi(argv[2]); //size of the input vector = number of lines in the file
  numthreads = atoi(argv[3]); //Third argument specifies number of threads to use for computing the solution
  pthread_t threadlist[numthreads]; //A list of all threads sent into the command line argument

  if(n < 2) //If there are less than two numbers for the prefix sum, then we cannot compute, and so we need to exit with EXIT_FAILURE.
  {
	exit(EXIT_FAILURE);
  }

  //Read in the contents (integers in this case) of the first argument file in the read_input_vector function
  input = malloc(sizeof(int) * n);
  read_input_vector(filename, n, input);

  //Calculate the prefix sum using a version of the Hillis and Steele Algorithm

  //Initialize the mutex using its virtual address
  //pthread_mutex_init(&lock, NULL);

  //Create each thread using a loop:
  for(int x = 0; x < sizeof(threadlist); x++)
  {
	 int result = pthread_create(&threadlist[x], NULL, calculateprefixsum(), NULL);

	 if(result != 0)
	 {
	   printf("\nThread cannot be created : [%s]", strerror(result));
	 }
  }

  //Block the calling thread until all threads created for the prefix_sum calculation terminate:
  for(int y = 0; y < sizeof(threadlist); y++)
  {
	  pthread_join(threadlist[y], NULL);
  }

  //Destroy the mutex lock since all threads created for the prefix_sum calculation have terminated:
  //pthread_mutex_destroy(&lock);

  //Print the Prefix Sum:

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, printPrefixSum, NULL);
  //Block the calling thread until thread_id terminates. Then, the calling thread can resume execution.
  pthread_join(thread_id, NULL);

  exit(0);
  fflush(stdout);
  return EXIT_SUCCESS;
}
