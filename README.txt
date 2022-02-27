Matthew McAlarney - memcalarney
Alison McNicholas - almcnicholas

To implement scan.c we used the Hillis Steele Algorithm, also known as the Parallel Prefic Scan Algorithm.
When taking inputs, the scan operation would run in a parallel function. 

We would first check to make sure there was 4 arguments so our functions would have all of the necessary inputs. 
Then, main would call read input vector which would put the input into an array of int pointers. After the
mutex lock and semaphores would be initialized. 
Next, each thread would be created by using pthread_create and the helper function calculateprefixsum. There was a 
two phased barrier which waited for all of the threads to arrive and to execute the critical section respectively.
The fnunction would then calculate the sum and then lock when it was completed. After all of this, the mutex lock
was destroyed because all of the threads created for the prefix sum has been terminated. 

Next, we would print the prefix sum. We would create a new thread calling the printPrefixSum function. Then we used pthed_join
to block the calling thread. When thread_id terminates, the calling thread would resume execution. Finally, we would free
the input and return a success. 
