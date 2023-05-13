#include "threading.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

// Optional: use these functions to add debug or error prints to your application
#define DEBUG_LOG(msg,...)
//#define DEBUG_LOG(msg,...) printf("threading: " msg "\n" , ##__VA_ARGS__)
#define ERROR_LOG(msg,...) printf("threading ERROR: " msg "\n" , ##__VA_ARGS__)

void* threadfunc(void* thread_param)
{
    struct thread_data* arguments = (struct thread_data*) thread_param;

    usleep(arguments->obtain * 1000);

    int result = pthread_mutex_lock(arguments->mutex);
    if (result) {
        ERROR_LOG("Failed to obtain mutex: %s\n", strerror(result));
        return NULL;
    }

    usleep(arguments->release * 1000);

    result = pthread_mutex_unlock(arguments->mutex);
    if (result) {
        ERROR_LOG("Failed to release mutex: %s\n", strerror(result));
        return NULL;
    }

    arguments->thread_complete_success = true;

    // TODO: wait, obtain mutex, wait, release mutex as described by thread_data structure
    // hint: use a cast like the one below to obtain thread arguments from your parameter
    return thread_param;
}


bool start_thread_obtaining_mutex(pthread_t *thread, pthread_mutex_t *mutex,int wait_to_obtain_ms, int wait_to_release_ms)
{
    /**
     * TODO: allocate memory for thread_data, setup mutex and wait arguments, pass thread_data to created thread
     * using threadfunc() as entry point.
     *
     * return true if successful.
     *
     * See implementation details in threading.h file comment block
     */
    struct thread_data* data = (struct thread_data*) malloc(sizeof(struct thread_data));
    if (!data) {
        ERROR_LOG("Failed to allocate memory for thread data\n");
        return false;
    }

    data->mutex = mutex;
    data->obtain = wait_to_obtain_ms;
    data->release = wait_to_release_ms;
    data->thread_complete_success = false;

    int result = pthread_create(thread, NULL, threadfunc, (void*) data);
    if (result) {
        ERROR_LOG("Failed to create thread: %s\n", strerror(result));
        free(data);
        return false;
    }

    return true;
}

