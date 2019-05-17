#define _BSD_SOURCE
#define NUM_ARGS 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

#define SERVER_PORT 4061

int status = 1;

struct mesg_buffer
{
	long mesg_type;
	char mesg_text[256];
} message;

struct buffer_t
{
	key_t key1;
	key_t key2;
};

int main(int argc, char **argv)
{

	if (argc > 1)
	{

		printf("Wrong number of args, expected 0, given %d\n", argc - 1);
		exit(1);
	}

	// Create a TCP socket.
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	key_t shmkey;
	int msgid;
	struct buffer_t *keyBuf;
	char *line = NULL;
	size_t len = 0;

	// Specify an address to connect to (we use the local host or 'loop-back' address).
	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(SERVER_PORT);
	address.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(sock, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Server has closed connection");
		exit(EXIT_FAILURE);
	}

	// ftok to generate unique keys
	shmkey = ftok("both", 65);
	//testing purposes only
	//printf("shmkey is %d\n", shmkey);

	// shmget returns an identifier in shmid
	int shmid = shmget(shmkey, sizeof(struct buffer_t *), 0666 | IPC_CREAT);
	if (shmid < 0)
	{
		perror("client shmget error");
		exit(EXIT_FAILURE);
	}

	// shmat to attach to shared memory
	if ((keyBuf = (struct buffer_t *)shmat(shmid, NULL, 0)) < 0)
	{
		perror("client shmat");
		exit(EXIT_FAILURE);
	}

	//printf("msgkey1 is %d\n", keyBuf->key1);


	// msgget creates a message queue
	// and returns identifier
	//this for msg


	msgid = msgget(keyBuf->key2, 0666 | IPC_CREAT);

	char buffer[] = "a";

	while (status) {


		int n = send(sock, buffer, 1, MSG_DONTWAIT);
		if(n < 0){
			status = 1;
		}

		if (msgrcv(msgid, (void *) &message, sizeof(message), 10, IPC_NOWAIT) >= 0){
			printf("Data Received is : %s \n", message.mesg_text);
		}

		if (msgrcv(msgid, (void *) &message, sizeof(message), 20, IPC_NOWAIT) >= 0) {
			printf("Data Received is : %s \n", message.mesg_text);
			//clear input buffer
			//while ((getchar()) != '\n');
			printf("Write response : ");

			if (getline(&line, &len, stdin) == -1) {
				perror("failed to read line");
			}
			strcpy(message.mesg_text, line);

			// msgsnd to send message
			if (msgsnd(msgid, &message, sizeof(message), 0) < 0) {
				perror("client msgsnd");
				exit(EXIT_FAILURE);
			}
		}
	}
	//detach from shared memory
	shmdt((void *) keyBuf);
	//close sock
	close(sock);
	return 0;
}
