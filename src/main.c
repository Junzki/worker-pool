/*
 * Copyright 2019 Andrew Junzki <andrew@junzki.me>
 *
 * The thread pool example in C with pthread.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#define DEFAULT_WORKERS 4

#define abort(message) do { \
    perror(message);        \
    exit(EXIT_FAILURE);     \
} while(0)


typedef struct {
    pthread_t thread_id;
    int       worker_id;
} worker_t;

typedef struct {
    int     task_id;
} task_t;

typedef struct {
    pthread_mutex_t mutex;
    size_t          size;
} task_queue_t;

static void* serial_output(void*);


int
main() {
    int index = 0;
    int stat  = 0;

    pthread_attr_t attr;
    worker_t contexts[DEFAULT_WORKERS];
    pthread_attr_init(&attr);

    while (index < DEFAULT_WORKERS) {
        worker_t *cxt = &(contexts[index]);
        cxt->worker_id = index;

        stat = pthread_create(&(cxt->thread_id), &attr, &serial_output, cxt);
        if (0 != stat) {
            abort("cannot init thread.");
        }

        ++index;
    }

    // Clean up
    pthread_attr_destroy(&attr);
    for (index = 0; index < DEFAULT_WORKERS; ++index) {
        void *res = NULL;

        worker_t* cxt = &(contexts[index]);

        stat = pthread_join(cxt->thread_id, &res);
        if (0 != stat) {
            abort("cannot join threads");
        }

        free(res);
        res = NULL;
    }

    return EXIT_SUCCESS;
}


static void*
serial_output(void *arg) {
    worker_t* cxt = arg;
    int delay = 0;

    int index = 0;
    while (index < 20) {
        delay = random() % 5;

        printf("Worker %d -- Index: %d, Delay: %d\n", cxt->worker_id, index, delay);
        sleep(delay);

        ++index;
    }

    return NULL;
}
