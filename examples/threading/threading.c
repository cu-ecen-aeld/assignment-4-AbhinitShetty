#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

// Optional: use these functions to add debug or error prints to your application
// #define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define DEBUG_LOG(msg,...)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)	

void* threadfunc(void* thread_param)
{
	struct thread_data *thread_func_args;
	thread_func_args = (struct thread_data*) thread_param;

	thread_func_args->thread = (pthread_t *)pthread_self();	// Get Process ID of the created thread
	
	sleep(thread_func_args->wait_to_obtain_ms);
	pthread_mutex_lock(thread_func_args->mutex);		// Obtain Mutex Lock

	sleep(thread_func_args->wait_to_release_ms);
	pthread_mutex_unlock((*thread_func_args).mutex);		// Release Mutex Lock
	
	thread_func_args->thread_complete_success = 1;
	
    return (void*)thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
	pthread_t th;
	struct thread_data *ptr;
	ptr = malloc(sizeof(struct thread_data));				// Dynamic Memory Allocation
	//printf("Address of ptr pointer is : %u\n", ptr);
	pthread_mutex_init (mutex, NULL);					// Initialize Mutex 
	
	if (pthread_create(&th, NULL, &threadfunc, ptr) != 0){			// Create a Thread
		perror("Error in Creating Thread !!");
		return false;
	}
		
	if (pthread_join(th, (void**) &ptr) != 0){				// Waiting for thread to complete its execution
		perror("Error at Destroying Thread !!");
		return false;
	}
	
	free(ptr);
	pthread_mutex_destroy (mutex);		// Destroy Mutex
	
    return true;
}

// TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure

/**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     
     
     */
     
     // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    //struct thread_data* thread_func_args = (struct thread_data *) thread_param;
    
    

