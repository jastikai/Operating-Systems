// Lab exercise task 1
// Jaakko Astikainen

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h> /* usage: bool var_name; possible values: true, false */

#define	NBUFFERS 2

int item, in=0, out=0;
int buffer[NBUFFERS];
int stop=0;
bool flag = false; // a boolean flag to mark if new item has been produced


void *ProducerTask(void *data)
{
	int nextp = 0;
	struct timespec mytime;
	mytime.tv_sec = 0;
	mytime.tv_nsec = 200000000;
     
	while (!stop) {
		nanosleep(&mytime, NULL);
		nextp++;
		buffer[in] = nextp;   /* produce a new item */
		in = (in + 1) % NBUFFERS;

		// make the task wait while the consumer task consumes the item
		flag = true;
		while(flag == true && !stop) {
			nanosleep(&mytime, NULL);
		}

		
	}
	
	pthread_exit(0);
}
void *ConsumerTask(void *data)
{
	int nextc;
	struct timespec mytime;
	mytime.tv_sec = 0;
	mytime.tv_nsec = 500000000;
     
	while (!stop) {
		nanosleep(&mytime, NULL);
		nextc = buffer[out];  /* consume a item */
		out = (out + 1) % NBUFFERS;
		printf("Consumed item: %d\n", nextc);

		// make the task wait while the producer task produces an item
		flag = false;
		while(flag == false && !stop) {
			nanosleep(&mytime, NULL);
		}



		
	}
	
	pthread_exit(0);
}

void *MonitorTask (void *data)
{
	getchar();
	stop = 1;
	pthread_exit(0);
}


int main(void)
{
	pthread_t task1;
	pthread_t task2;
	pthread_t task3;

	pthread_create (&task1, NULL, ProducerTask, NULL);
	pthread_create (&task2, NULL, ConsumerTask, NULL);
	pthread_create (&task3, NULL, MonitorTask, NULL);

	pthread_join(task1,NULL);
	pthread_join(task2,NULL);
	pthread_join(task3,NULL);

	printf("Main program exiting.\n");

	return 0;
}