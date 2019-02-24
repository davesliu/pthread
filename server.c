#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


#define NUM_THREADS   4

static int loop_cnt = 10000;
static int count = 0;

static pthread_attr_t   attrs;
static pthread_mutex_t  mtx;
static pthread_cond_t   cdv;

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

    for (int i = 0; i < loop_cnt; i++)
    {
        pthread_mutex_lock(&mtx);
        count++;

        pthread_cond_signal(&cdv);

        pthread_mutex_unlock(&mtx);
    }

    pthread_exit(args);
}

void QuaryTask(void *args)
{
    printf("[Thread 0x%lx]enter QuaryTask\n", pthread_self());

    for (int i = 0; i < loop_cnt; i++)
    {
        pthread_mutex_lock(&mtx);
        count++;
        pthread_mutex_unlock(&mtx);
    }

    pthread_cond_signal(&cdv);

    pthread_exit(args);
}


void CommTask(void *args)
{
    printf("[Thread 0x%lx]enter CommTask\n", pthread_self());

    for (int i = 0; i < loop_cnt; i++)
    {
        pthread_mutex_lock(&mtx);
        count++;
        pthread_mutex_unlock(&mtx);
    }

    pthread_cond_signal(&cdv);

    pthread_exit(args);
}

void RefreshTask(void *args)
{
    printf("[Thread 0x%lx]enter RefreshTask\n", pthread_self());

    pthread_mutex_lock(&mtx);

    while(count < loop_cnt * 3)
    {
        pthread_cond_wait(&cdv, &mtx);
    }

    count += loop_cnt;

    pthread_mutex_unlock(&mtx);

    pthread_exit(args);
}

void InitMainTask()
{
    printf("Init MainTask\n");

    pthread_attr_init(&attrs);
    pthread_attr_setdetachstate(&attrs, PTHREAD_CREATE_JOINABLE);

    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cdv, NULL);
}

void LoopMainTask()
{
    int cur_ret;

    printf("Loop MainTask\n");
}

void ExitMainTask()
{
    printf("Exit MainTask\n");

    pthread_attr_destroy(&attrs);
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cdv);
    pthread_exit(NULL);
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
        ret = pthread_create(&threads[i], &attrs, task_func[i], (void *)(long)i);
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

    printf ("count = %d\n", count);

}

