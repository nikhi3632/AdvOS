/* CSE 536: User-Level Threading Library */
#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/syscall.h"
#include "user/user.h"
#include "user/ulthread.h"

/* Standard definitions */
#include <stdbool.h>
#include <stddef.h> 

/* Global Variables */
struct ulthread ulthreads[MAXULTHREADS];
int num_threads = 0;
struct ulthread *ul_thread;
int current_ulthread_idx = 0;

/* Get thread ID */
int get_current_tid(void) {
    return ul_thread->tid;
}

/* Thread initialization */
void ulthread_init(int schedalgo) {
    ul_thread = &ulthreads[current_ulthread_idx];
    ul_thread->state = RUNNABLE;
    ul_thread->sched_algo = schedalgo;
    ul_thread->tid = 0;
    current_ulthread_idx = 1;
}

/* Thread creation */
bool ulthread_create(uint64 start, uint64 stack, uint64 args[], int priority) {
    /* Please add thread-id instead of '0' here. */
    printf("[*] ultcreate(tid: %d, ra: %p, sp: %p)\n", current_ulthread_idx, start, stack);
    ul_thread = &ulthreads[current_ulthread_idx];
    ul_thread->state = RUNNABLE;
    ul_thread->tid = current_ulthread_idx;
    ul_thread->priority = priority;
    memset(&ul_thread->ult_context, 0, sizeof ul_thread->ult_context);
    ul_thread->ult_context.ra = start;
    ul_thread->ult_context.sp = stack;
    ul_thread->ult_context.arg0 = args[0];
    ul_thread->ult_context.arg1 = args[1];
    ul_thread->ult_context.arg2 = args[2];
    ul_thread->ult_context.arg3 = args[3];
    ul_thread->ult_context.arg4 = args[4];
    ul_thread->ult_context.arg5 = args[5];

    if(ulthreads[0].sched_algo == FCFS) {
        ul_thread->c_time = ctime(); // ctime required for FCFS
    }

    num_threads++;
    current_ulthread_idx++;
    return true;
}

struct ulthread* handle_fcfs() {
    struct ulthread *next_thread = NULL;
    int thread_clock_time = __INT32_MAX__;
    int thread_index = -1;

    for(int i = 1; i <= num_threads; i++) {
        if((ulthreads[i].state == RUNNABLE) && (ulthreads[i].c_time < thread_clock_time)){ //check ctime for fcfs scheduling
            thread_clock_time = ulthreads[i].c_time;
            thread_index = i;
        }
    }
    next_thread = (thread_index > 0) ? &ulthreads[thread_index] : next_thread;
    return next_thread;
}

struct ulthread* handle_priority() {
    struct ulthread *next_thread = NULL;
    int thread_prority = -1;
    int thread_index = -1;

    for(int i = 1; i <= num_threads; i++) {
        if((ulthreads[i].state == RUNNABLE) && (thread_prority <= ulthreads[i].priority)){ //check priority for priority scheduling
            thread_prority = ulthreads[i].priority;
            thread_index = i;
        }
    }
    next_thread = (thread_index > 0) ? &ulthreads[thread_index] : next_thread;
    return next_thread;
}

struct ulthread* handle_roundrobin() {
    int idx;
    int thread_index = 0;

    for(int i = 0; i < num_threads; i++) {
        int round_robin_index = (thread_index + i) % (num_threads + 1); //allocate round robin for round robin scheduling
        if(round_robin_index) {
            idx = round_robin_index;
        } else {
            idx = 1;
            thread_index += 1;
        }
        if(ulthreads[idx].state == RUNNABLE) {
            return &ulthreads[idx];
        }
    }
    return NULL;
}

struct ulthread* track_yeild() {
    for(int i = 1; i <= num_threads; i++) {
        if(ulthreads[i].state == RUNNABLE) {
            return &ulthreads[i];
        }
    }
    return NULL;
}

/* Thread scheduler */
void ulthread_schedule(void) {

    while(num_threads > 0) {
        struct ulthread *next_thread = NULL;
        if(ulthreads[0].sched_algo == FCFS) {
            next_thread = handle_fcfs();
        } 
        else if(ulthreads[0].sched_algo == PRIORITY) {
            next_thread = handle_priority();
        }
        else if(ulthreads[0].sched_algo == ROUNDROBIN) {
            next_thread = handle_roundrobin();
        }

        if(ul_thread->state == YIELD) {
            ul_thread->state = RUNNABLE;
        }
        
        if(next_thread == NULL) {
            next_thread = track_yeild();
        }

        if(next_thread) {
            ul_thread = next_thread;
        }
        else {
            // after tracking yeild
            break;
        }
        printf("[*] ultschedule (next tid: %d)\n", next_thread->tid);
        ulthread_context_switch(&ulthreads[0].ult_context, &next_thread->ult_context);
    }                                      
}

/* Yield CPU time to some other thread. */
void ulthread_yield(void) {
    ul_thread->state=YIELD;
    printf("[*] ultyield(tid: %d)\n", ul_thread->tid);
    struct ulthread *sched_thread = &ulthreads[0];
    ulthread_context_switch(&ul_thread->ult_context, &sched_thread->ult_context);
}

/* Destroy thread */
void ulthread_destroy(void) {
    ul_thread->state = FREE;
    num_threads--;
    printf("[*] ultdestroy(tid: %d)\n", ul_thread->tid);
    struct ulthread *sched_thread = &ulthreads[0];
    ulthread_context_switch(&ul_thread->ult_context, &sched_thread->ult_context);
}
