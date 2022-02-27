Matthew McAlarney - memcalarney
Alison McNicholas - almcnicholas

General Overview of Program Design:
To implement scan.c we use the Hillis Steele Algorithm with a 2-Phased barrier, also known as the Parallel Prefix Scan Algorithm.
Inside our calculateprefixsum() function, we implement both the Hillis Steele Algorithm and the 2-Phased barrier.
When read a series of lines (where each line contains a number) from the command line argument input into our input vector in our read_input_vector() function. 
Inside our main() function, we read in the command line arguments, and store those values in a combinatin of local and global variables.
Inside our main() function, we also check to make sure there are exactly 4 command line arguments, 2 or more values in n for the prefix sum calculation, read values into the input vector with a call to read_input_vector(), initilaize our global mutex lock and semaphores, create each thread, and join all of them together.
Lastly in main(), we print out the calculated prefix sum through a call to the function printPrefixSum() using an additional thread for extra practice with C's pthread.h header file.

Step-By-Step Explanation of General Program Logic:

We check to make sure there are 4 command line arguments so our read_input_vector() and calculateprefixsum() functions have all of the necessary inputs (program name, pointer to the input file, size of the input vector/number of lines in the input file, and the number of threads. 
If there are 4 arguments, we store those arguments in a combination of local and global variables. If not, we exit with EXIT_FAULRE.
If there are less than two numbers inputed to n, then we cannot calculate the prefix sum, and we exit with EXIT_FAILURE.
Initialize the threadlist using the number of threads sent in on the command line.
Assign the input array memory on the heap through malloc().
Call read_input_vector() to to read values from the file pointer into the input array.
Initialize the mutex lock, and initialize two semaphores.
Set the maximum number of threads to equal the number of threads read from the command line for later use in calculateprefixsum().
Loop through the threadlist, create each thread, and send each one to the calculateprefixsum() function to complete a portion of the work in calculating the prefix sum.
If a thread cannot be created, indicate this in the terminal with a printed message.
Loop through the thread list and join eahc thread. The calling thread resumes execution once every thread has finished computing their portion of the prefix sum calculation in function calculateprefixsum().
Destroy the global mutex lock.
Create a new thread for extra practice, and send it to execute the printPrefixSum() function to print the prefix sum.
Join this thread, and the calling thread will resume execution once the prefix sum as been printed to the console.
Free the input array to remove the dynamically allocated memory from the heap.
Return EXIT_SUCCESS.

How our design divides data and computation between the threads:

When thread make their way past the first barrier of the 2-phased barrier surrounding the Hillis Steele implementation in calculateprefixsum(), one thread or more need to first check the condition that the prefix sum calculation is complete (prefixsumparallelcomplete == 1). If the calculation is not complete (prefixsumparallelcomplete == 0), then one thread or more enters further into the critical section and checks another condition; if one thread or more is able to move onto the next computation level (moveontonextcomputationlevel == 1), then one or more threads enter the current iteration or level of computation of the Hillis Steele Algorithm (dictated by the interval between input array entries; the current power of n in 2^n). Global variable moveontonextcomputationlevel is flipped back to a value of zero to signal to other threads that have just entered the critical section that the current iteration of the Hillis Steele algorithm is still in the process of completion. One thread or more computes the individual sums in the current iteration of the input array/level of computation until the last index is accounted for. There is an array out of bounds check to make sure that the current array index plus the interval between indexes does not exceed the last index in the input array. There is also a check to see if the number of steps (a step is a single sum in the prefix sum calculation) is equal to log2(n). In this case, the prefix sum calculation is finished, and no other threads need to maniupulate the input array. 

How our design implements concurrency mechnaisms to ensure correctness, avoid deadlocks, and coordinate threads:




