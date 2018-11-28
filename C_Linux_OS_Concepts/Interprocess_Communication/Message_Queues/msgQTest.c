/*Blake Simmons
  CIS 370
  Lab - 9 - msgQTest.c
  November 13, 2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include "message.h"


//function to print usage information upon error
void printUsage(){
	printf("Error: Invalid arguments\n"); 
	printf("Usage:\n");
	printf("(Create Queue) <-c/C> <key>\n");
	printf("(Send a message) <-s/S> <key> <recipient_id> <message>\n"); 
	printf("(Receive a message) <-r/R> <key> <recipient_id>\n"); 		printf("(Delete queue) <-d/D> <key>\n");
}


//main
int main(int argc, char **argv){

//declare necessary variables
char flag;
int msqid;
messageBuffer mbuf;
size_t buf_length;
int msgflg = IPC_CREAT | 0666;
key_t key;
unsigned long recipient;



//validate arguments
if((argc < 3 || argc > 5) || argv[1][0] != '-' || strlen(argv[1]) != 2){		
	printUsage();
	return -1;	
}

flag = argv[1][1];


switch(flag){



		//Create a message queue
	case 'c':
	case 'C':
		if((key = atoi(argv[2])) < 1){
			perror("Error assigning key");
			return -1;
		}
		if((msqid = msgget(key, msgflg)) < 1){
			perror("Error creating queue");
			return -1;
		}
		printf("%s%i%s\n", "Message queue ", msqid, " created.");
		break;



		//Send a message
	case 's':
	case 'S':
		if(argc == 5){
			if((key = atoi(argv[2])) < 1){
				perror("Error assigning key");
				return -1;
			}
			if((msqid = msgget(key, 0400)) < 1){
				perror("Error accessing queue");
			}
			mbuf.recipient = atoi(argv[3]);
			strncpy(mbuf.msg, argv[4], MSG_MAX);
			buf_length = strlen(mbuf.msg) + 1;
	
			if(msgsnd(msqid, &mbuf, (sizeof(mbuf.msg) + 1), IPC_NOWAIT) < 0) 			{ 		
				perror("Error sending message");
				return -1;
			}	
			printf("Message sent (%lu): %s\n", mbuf.recipient, mbuf.msg);
		}
		else{
			printUsage();
		}

		break;



		//Receive a message
	case 'r':
	case 'R':
		
		if(argc == 4){
			if((key = atoi(argv[2])) < 1){
				perror("Error assigning key");
				return -1;
			}
			if((msqid = msgget(key, 0400)) < 1){
				perror("Error accessing queue");
			}
			recipient = atoi(argv[3]);
			if( msgrcv(msqid, &mbuf, sizeof(mbuf)-sizeof(long), recipient, IPC_NOWAIT) < 0){
				perror("Error receiving message");
				return -1;
			}
			printf("Message received (%lu): %s\n", mbuf.recipient, mbuf.msg);
		}
		else{
			printUsage();
		}

		break;
	
	

		//Delete a message queue
	case 'd':
	case 'D':
		if((key = atoi(argv[2])) < 1){
			perror("Error assigning key");
			return -1;
		}
		if((msqid = msgget(key, 0400)) < 1){
			perror("Error accessing queue");
		}
		

		printf("%s%d%s\n", "Message queue ", msqid, " deleted.");
		if((msgctl(msqid, IPC_RMID, NULL)) < 0){
			perror("Delete message queue failed");
		}
		break;	



		//invalid flag
	default:
		printUsage();
		return -1;		
}






return 0;
}
