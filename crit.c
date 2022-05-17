// Lab exercise task 3
// Jaakko Astikainen

#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdbool.h> /* usage: bool var_name; possible values: true, false */

char table[26];
int stop=0;
//bool flag = false; // boolean flag to mark permission to access critical section
int turn = 0;
bool flag[2] = {false, false};

void *Task1(void *data)
{
	int cnt=0,c=0;
	char ch;

	struct timespec mytime;
	mytime.tv_sec = 0;
	mytime.tv_nsec = 10000000;
	while (!stop) {
		nanosleep(&mytime,NULL);

		// Peterson's algorithm
		flag[0] = true;
		turn = 1;
		while(flag[1] && turn == 1) {
			nanosleep(&mytime,NULL); // wait for the other task
		}

		// critical section
		printf ("\nTask1 writing: ");
		for(cnt=0; cnt<26; cnt++) {
			nanosleep(&mytime,NULL);
			ch = 'a' + ((cnt + c) % 26);
			table[cnt] = ch;
			printf("%c",ch);
		}

		flag[0] = false;
		
		//remainder section
		c++;

		//Screen is now available!!
		nanosleep(&mytime,NULL); /* remainder section - do nothing... */

	}
	
	pthread_exit(0);
         
}
void *Task2(void *data)
{
	int cnt=0,c=0;
	char ch;

	struct timespec mytime;
	mytime.tv_sec = 0;
	mytime.tv_nsec = 100000;

	while (!stop) {
		mytime.tv_nsec = 1000000;
		nanosleep(&mytime,NULL);

		// Peterson's algorithm
		flag[1] = true;
		turn = 0;
		while(flag[0] && turn == 0) {
			nanosleep(&mytime,NULL); // wait for the other task
		}

		// critical section
		printf("\nTask2 writing: ");
		for(cnt=0; cnt<26; cnt++) {
			nanosleep(&mytime,NULL);
			ch = '1' + ((cnt + c) % 9);
			table[cnt] = ch;
			printf("%c",ch);
		}
		
		flag[1] = false;

		//remainder section
		c++;
		
		mytime.tv_nsec = 800000000;
		nanosleep(&mytime,NULL); /* remainder section - do nothing... */
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

	pthread_create (&task1, NULL, Task1, NULL);
	pthread_create (&task2, NULL, Task2, NULL);
	pthread_create (&task3, NULL, MonitorTask, NULL);

	pthread_join(task1,NULL);
	pthread_join(task2,NULL);
	pthread_join(task3,NULL);

	printf("Main program exiting.\n");

	return 0;
}