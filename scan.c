#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define MAX_LINE_SIZE 256

//Globals:

int *input; //shared resource among all threads
int n = 0;
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

void calculatePrefixSum(int n)
{
	//Critical Section
	for(int i = 0; i < n; i++)
	{
		input[i] += input[i-1];
	}
}

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
  //int
   n = atoi(argv[2]); //size of the input vector = number of lines in the file
  //int numThreads = argv[3]; //Third argument specifies number of threads to use for computing the solution

  if(n < 2) //If there are less than two numbers for the prefix sum, then we cannot compute, and so we need to exit with EXIT_FAILURE.
  {
	exit(EXIT_FAILURE);
  }

  input = malloc(sizeof(int) * n); //Used to read in the contents (integers in this case) of the first argument file in the read_input_vector function
  read_input_vector(filename, n, input);

  //Print the prefix sum
  calculatePrefixSum(n);

pthread_t thread_id;
	
	pthread_create(&thread_id, NULL, printPrefixSum, NULL);
	pthread_join(thread_id, NULL);
	
	exit(0);
	fflush(stdout);
  return EXIT_SUCCESS;
}
