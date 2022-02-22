#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#define MAX_LINE_SIZE 256

//Partial Sum Approach

//Fully compiles

//Globals:

int *input; //shared resource among all threads
int n = 0; //shared resource among all threads
int numThreads = 0; //shared resource among all threads
pthread_mutex_t lock; //Declare the mutex for locking purposes

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
void* calculatePrefixSum(int startindex, int partition)
{
	//Provide a lock to the current thread that has entered the critical section so that no other locks can enter at the same time
	pthread_mutex_lock(&lock);

	//Critical Section

	if((startindex + partition) <= (sizeof(input) - 1))
	{
		for(int i = startindex; i < partition; i++)
		{
			input[i] += input[i-1];
		}
	}
	else
	{
		partition = (sizeof(input) - 1) - startindex;

		for(int i = 0; i < partition; i++)
		{
			input[i] += input[i - 1];
		}
	}

	//Release the lock so that other threads now have a chance to enter the critical section:
	pthread_mutex_unlock(&lock);

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
  numThreads = atoi(argv[3]); //Third argument specifies number of threads to use for computing the solution
  pthread_t threadlist[numThreads]; //A list of all threads sent into the command line argument
  int partition = 0; //Denotes a subsection of fullsection
  int startindex = 1; //Denotes the current starting index inside the input array

  if(n < 2) //If there are less than two numbers for the prefix sum, then we cannot compute, and so we need to exit with EXIT_FAILURE.
  {
	exit(EXIT_FAILURE);
  }

  //Read in the contents (integers in this case) of the first argument file in the read_input_vector function
  input = malloc(sizeof(int) * n);
  read_input_vector(filename, n, input);

  //Calculate the prefix sum

  //Initialize the mutex using its virtual address
  pthread_mutex_init(&lock, NULL);

  //Assign the partition:
  partition = (sizeof(input) - 1) / sizeof(threadlist);

  //Create each thread using a loop:
  for(int x = 0; x < sizeof(threadlist); x++)
  {
	 int result = pthread_create(&threadlist[x], NULL, calculatePrefixSum(startindex, partition), NULL);

	 if(result != 0)
	 {
	   printf("\nThread cannot be created : [%s]", strerror(result));
	 }

	 startindex = startindex + partition;
  }

  //Block the calling thread until all threads created for the prefix_sum calculation terminate:
  for(int y = 0; y < sizeof(threadlist); y++)
  {
	  pthread_join(threadlist[y], NULL);
  }

  //Destroy the mutex lock since all threads created for the prefix_sum calculation have terminated:
  pthread_mutex_destroy(&lock);

  //Print the Prefix Sum:

  pthread_t thread_id;
  pthread_create(&thread_id, NULL, printPrefixSum, NULL);
  //Block the calling thread until thread_id terminates. Then, the calling thread can resume execution.
  pthread_join(thread_id, NULL);

  exit(0);
  fflush(stdout);
  return EXIT_SUCCESS;
}
