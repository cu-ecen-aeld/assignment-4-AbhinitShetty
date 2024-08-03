#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


void* threadfunc(void* thread_param)
{
	struct thread_data *thread_func_args = (struct thread_data*) thread_param;
	
	if (usleep(thread_func_args->wait_to_obtain_ms * 1000) != 0){		// Delay in microseconds 
		return thread_func_args;
	}
	
	if (pthread_mutex_lock(thread_func_args->mutex) != 0){		// Obtain Mutex Lock
		return thread_func_args;
	}
	
	if (usleep(thread_func_args->wait_to_obtain_ms * 1000) != 0){
		pthread_mutex_unlock(thread_func_args->mutex);		// Release Mutex Lock
		return thread_func_args;
	}
	
	thread_func_args->thread_complete_success = true;		// Creation of Thread successfull 
	pthread_mutex_unlock(thread_func_args->mutex);			// Release Mutex Lock
	
    return thread_func_args;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex, int wait_to_obtain_ms, int wait_to_release_ms)
{
	if (wait_to_obtain_ms < 0 || wait_to_release_ms < 0){			// Check if Delay values are permissible 
        	return false;
    	}
    	
	struct thread_data *ptr = malloc(sizeof(struct thread_data));	 	// Dynamic Memory Allocation
	if (!ptr){
        	return false;
    	}
	
	// Assign mutex values to the Dyn. Mem. allocated pointer
	ptr->mutex = mutex;
	ptr->wait_to_obtain_ms = wait_to_obtain_ms;
	ptr->wait_to_release_ms = wait_to_release_ms;
	ptr->thread_complete_success = false;
	
	//pthread_mutex_init (ptr->mutex, NULL);				// Error : Initialize Mutex Lock only in the main 'C' file 
	
	int thread_value = pthread_create(thread, NULL, threadfunc, ptr);
	
	if (thread_value != 0){							// Create a Thread
		free(ptr);
	}
	
	//pthread_mutex_destroy (ptr->mutex);					// Destroy Mutex
	
    return thread_value == 0;
}

    
    /*if (pthread_join(*thread, (void**) &ptr) != 0){				// Waiting for thread to complete its execution
		perror("Error at Destroying Thread !!");
		return false;
	}*/

