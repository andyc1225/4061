#define _BSD_SOURCE
#define NUM_ARGS 0

#include <stdio.h>
#include <stdlib.h>
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
#include <errno.h>
#include <fcntl.h>


#define SERVER_PORT 4061
#define MAX_CONNECTIONS 100

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

    if (argc > NUM_ARGS + 1)
    {

        printf("Wrong number of args, expected %d, given %d\n", NUM_ARGS, argc - 1);
        exit(1);
    }

    // Create a TCP socket.
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0){
        perror("could not create socket");
        exit(EXIT_FAILURE);
    }

    // Bind it to a local address.
    struct sockaddr_in servAddress;
    servAddress.sin_family = AF_INET;
    servAddress.sin_port = htons(SERVER_PORT);
    servAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr *)&servAddress, sizeof(servAddress)) < 0){
        perror("could not bind");
        exit(EXIT_FAILURE);
    }


    int flags = fcntl(sock, F_GETFL);

    //set sock accept to be non-blocking so that the while loop can run and show the menu
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) < 0){
        perror("could not set flags");
        exit(EXIT_FAILURE);
    }

    // TODO: Listen on this socket.
    if (listen(sock, MAX_CONNECTIONS) < 0)
    {
        printf("connection failed");
        return 0;
    }



    struct buffer_t *keyBuf;
    int msgid1, msgid2;
    int acpt[MAX_CONNECTIONS];

    // ftok to generate unique keys
    // Use both for shared mem key, cli1 for client 1, cli2 for client 2
    key_t shmkey = ftok("both", 65);
    //printf("shmkey is %d\n", shmkey);

    // shmget returns an identifier in shmid
    int shmid = shmget(shmkey, sizeof(struct buffer_t), 0666 | IPC_CREAT);
    if (shmid < 0)
    {
        perror("server shmget error");
        exit(EXIT_FAILURE);
    }

    // shmat to attach to shared memory
    if ((keyBuf = (struct buffer_t *)shmat(shmid, NULL, 0)) < 0)
    {
        perror("server shmat");
        exit(EXIT_FAILURE);
    }

    //set shared mem common keys
    keyBuf->key1 = ftok("cli1", 65);
    keyBuf->key2 = ftok("cli2", 65);


    //printf("msgkey1 is %d\n", keyBuf->key1);
    //printf("msgkey2 is %d\n", keyBuf->key2);

    // msg queue stuff
    msgid1 = msgget(keyBuf->key1, 0666 | IPC_CREAT);
    if(msgid1 < 0){
        perror("msgget failed id 1");
        exit(EXIT_FAILURE);
    }
    msgid2 = msgget(keyBuf->key2, 0666 | IPC_CREAT);
    if(msgid2 < 0){
        perror("msgget failed id 1");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    int tempacpt;

    // A server typically runs infinitely, with some boolean flag to terminate.
    while (status)
    {

        int input;

        char *line = NULL;
        size_t len = 0;
        // Now accept the incoming connections.
        struct sockaddr_in clientAddress;

        socklen_t size = sizeof(struct sockaddr_in);

        // TODO: Accept a connection.
        if ((tempacpt = accept(sock, (struct sockaddr *)&clientAddress, &size)) < 0)
        {
            if (errno != EWOULDBLOCK) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
        }
        else if(tempacpt >= 0){
            acpt[count++] = tempacpt;
        }


        // TODO: send msg to socket
        printf("Enter 1 to choose client 1\n2 to choose client 2\n3 to broadcast a message\n0 to exit\n");
        scanf("%d", &input);
        printf("\n");

        switch (input)
        {
        case 0:
            printf("exiting\n");
            status = 0;
            for(int i = 0; i < count; i++){close(acpt[i]);}
            break;
        case 1:
            message.mesg_type = 20;
            //clear input buffer
            while ((getchar()) != '\n');
            printf("Enter message :\n");

            if (getline(&line, &len, stdin) == -1)
            {
                perror("failed to read line");
            }

            strcpy(message.mesg_text, line);
            msgsnd(msgid1, &message, sizeof(message), 0);
            printf("Data sent by m is : %s \n", message.mesg_text);
            msgrcv(msgid1, (void *) &message, sizeof(message), 20, 0);
            break;
        case 2:
            message.mesg_type = 20;
            while ((getchar()) != '\n');
            printf("Enter message : ");

            if (getline(&line, &len, stdin) == -1)
            {
                perror("failed to read line");
            }

            strcpy(message.mesg_text, line);
            msgsnd(msgid2, &message, sizeof(message), 0);
            printf("Data sent by m is : %s \n", message.mesg_text);
            msgrcv(msgid2, (void *) &message, sizeof(message), 20, 0);
            break;
        case 3:
            message.mesg_type = 10;
            while ((getchar()) != '\n');
            printf("Enter message : ");

            if (getline(&line, &len, stdin) == -1)
            {
                perror("failed to read line");
            }

            strcpy(message.mesg_text, line);
            printf("past string copy\n");
            msgsnd(msgid1, &message, sizeof(message), 0);
            msgsnd(msgid2, &message, sizeof(message), 0);
            break;
        default:
            printf("option doesn't exist\n");
            break;
        }
    }
    //detach from shared mem
    shmdt((void *)keyBuf);
    // Close the server socket.
    close(sock);
    return 0;
}
