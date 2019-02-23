#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_THREADS   4

void ControlTask(void *args);
void QuaryTask(void *args);
void CommTask(void *args);
void RefreshTask(void *args);

void* task_func[NUM_THREADS] =
{
    ControlTask,
    QuaryTask,
    CommTask,
    RefreshTask
};

void ControlTask(void *args)
{
    printf("[Thread 0x%lx]enter ControlTask\n", pthread_self());


    pthread_exit(args);
}

void QuaryTask(void *args)
{
    printf("[Thread 0x%lx]enter QuaryTask\n", pthread_self());


    pthread_exit(args);
}


void CommTask(void *args)
{
    printf("[Thread 0x%lx]enter CommTask\n", pthread_self());


    pthread_exit(args);
}

void RefreshTask(void *args)
{
    printf("[Thread 0x%lx]enter RefreshTask\n", pthread_self());


    pthread_exit(args);
}

void InitMainTask()
{
    printf("Init MainTask\n");


}

void LoopMainTask()
{
    printf("Loop MainTask\n");
}


int main (int argc, char *argv[])
{
    pthread_t threads[NUM_THREADS];
    int ret;
    int i;
    void *status;

    InitMainTask();

    for(i = 0; i < NUM_THREADS; i++)
    {
        printf("In MainTask: creating thread %d\n", i);
        ret = pthread_create(&threads[i], NULL, task_func[i], (void *)(long)i);
        if (ret)
        {
            printf("ERROR; return code from pthread_create() is %d\n", ret);
            exit(-1);
        }
    }

    LoopMainTask();


    for(i = 0; i < NUM_THREADS; i++)
    {
        ret = pthread_join(threads[i], &status);
        if (ret)
        {
            printf("ERROR; return code from pthread_join() is %d\n", ret);
            exit(-1);
        }

        printf("MainTask: completed join with thread %d having a status of %ld\n", i, (long)status);
    }


    printf("Exit MainTask\n");

    pthread_exit(NULL);
}

