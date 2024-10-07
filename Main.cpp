#include <iostream>
#include <pthread.h>
#include <stdio.h>

using namespace std;

pthread_cond_t read_phase =PTHREAD_COND_INITIALIZER;
pthread_cond_t write_phase = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex;
int shared_resource = 1;
int resource_counter = 0;

void *writer(void *writer_num) 
{
  while(1) {
    pthread_mutex_lock(&mutex);
    while(resource_counter != 0)
        pthread_cond_wait(&write_phase, &mutex);
    resource_counter = -1;
    pthread_mutex_unlock(&mutex);
    shared_resource = shared_resource + 5;
    printf("Writer %d is modifying shared resource to %d\n",(*((int
    *)writer_num)),shared_resource);
    pthread_mutex_lock(&mutex);
    resource_counter = 0;
    pthread_cond_broadcast(&read_phase);
    pthread_cond_signal(&write_phase);
    pthread_mutex_unlock(&mutex);
    }
}

void *reader(void *reader_num)
{
    while(1){
    pthread_mutex_lock(&mutex);
    while(resource_counter == -1)
    pthread_cond_wait(&read_phase, &mutex);
    resource_counter++;
    pthread_mutex_unlock(&mutex);
    printf("Reader %d: reading shared resource as %d\n",*((int
    *)reader_num),shared_resource);
    pthread_mutex_lock(&mutex);
    resource_counter--;
    if(resource_counter == 0)
    pthread_cond_signal(&write_phase);
    pthread_mutex_unlock(&mutex);
    }
}
  

int main() {
  pthread_t read[2],write[1];
  pthread_mutex_init(&mutex, NULL);

  int a[2] = {1,2};
}
