// Lab exercise task 6
// Jaakko Astikainen

#include <stdio.h> 
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>

//#define NULL 0
#define THEIRPORT 6969    /* choose the port (in range 6900-6999) users will be sending to */
#define MYPORT 4020  /* choose a port (in range 4000-4099) from which to send */
#define MAXBUFLEN 100

int main(int argc, char *argv[])
{
	int pid;
	int UDPSocket;
	struct sockaddr_in their_addr, my_addr; /* connector's address information */
	struct hostent *he;
	int numbytes;
	char buffer[MAXBUFLEN];
	int addr_len;

	if (argc != 2) {
		fprintf(stderr,"usage: ./fork_socket message\n");
		return 0; //exit(1);
	}

	/* Create child process */
	pid = fork();
	if (pid == 0) /* This is the child process */
	{	
		execl("./child_socket",NULL,NULL); /* Load child's program code */
		return 0; //exit(0); /* Should never get here, terminate */
	}
	else {
		/* Parent code starts here */
		printf("\n\nProcess[%d]: Parent executing...\n", getpid());     
		printf("Process[%d]: I created a child (Pid = %d)\n",getpid(),pid);          
		sleep (4);
	}

	/* Initialize own address information */
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr =  INADDR_ANY;
	my_addr.sin_port = htons(MYPORT);
	bzero(&(my_addr.sin_zero),8);

	/* Create socket */
	if ((UDPSocket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		return 0; //exit(1);
	}
	if (bind(UDPSocket, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1){ 
		perror("bind");
		return 0; // exit(1);
	}

	/* Initialize recipient's address information */
	their_addr.sin_family = AF_INET;      /* host byte order */
	their_addr.sin_port = htons(THEIRPORT);  /* short, network byte order */
	/* their_addr.sin_addr = *((struct in_addr *)he->h_addr); */
	their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	bzero(&(their_addr.sin_zero), 8);     /* zero the rest of the struct */

	/* Send message */
	if ((numbytes=sendto(UDPSocket, argv[1], strlen(argv[1]), 0, \
			(struct sockaddr *)&their_addr, sizeof(struct sockaddr))) == -1) {
		perror("sendto");
		return 0; //exit(1);
	}

	printf("Process[%d]: Sent %d bytes to port %d in %s\n", getpid(),numbytes, ntohs(their_addr.sin_port), inet_ntoa(their_addr.sin_addr));
	/*Parent waits for the child to terminate */

	if (wait(&pid) > 0) { /* A child terminating */
		printf("Process[%d]: Parent detects terminating child\n", getpid());
	}
	
	printf("process[%d]: Parent terminating ...\n",getpid());

	return 0;
}

     

