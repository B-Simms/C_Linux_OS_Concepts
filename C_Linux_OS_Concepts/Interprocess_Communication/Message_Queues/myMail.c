/*Blake Simmons
  CIS 370
  Lab - 9 - myMail.c
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

#define MAX_ARGS 20
#define MAX_ARG_LENGTH 42
#define MAX_INPUT_LENGTH 50 


//function to print usage information upon error
void printUsage(){
	printf("Error: Invalid arguments\n"); 
	printf("Usage:\n");
	printf("(Create Queue) <-c/C> <key>\n");
	printf("(Delete queue) <-d/D> <key>\n");
	printf("(Login to Mail Service) <-l/L> <user_id> <key>\n");

}


//main
int main(int argc, char **argv){

	//declare necessary variables
	char flag;
	int j = 0;
	int msqid;
	messageBuffer mbuf;
	struct msqid_ds msgQueue;
	size_t buf_length;
	int msgflg = IPC_CREAT | 0666;
	key_t key;
	unsigned long recipient;
	unsigned long user;
	char input[MSG_MAX];
	char exit[] = "exit\n\0";
	char **tokenized;
	char *token;
	
	
	
	//validate arguments
	if((argc < 3 || argc > 5) || argv[1][0] != '-' || strlen(argv[1]) != 2){	
		printUsage();
		return -1;	
	}
	
	//capture character value only from flag argument
	flag = argv[1][1];
	
	//determine action based on flag argument
	switch(flag){
	
	
	
			//Create a message queue
		case 'c':
		case 'C':

			//get key from args
			if((key = atoi(argv[2])) < 1){
				perror("Error assigning key");
				return -1;
			}

			//use key to create message queue if queue does not exist
			if((msqid = msgget(key, msgflg)) < 1){
				perror("Error creating queue");
				return -1;
			}
			printf("%s%i%s\n", "Message queue ", msqid, " created.");
			break;
	
		
			//Login to service
		case 'l':
		case 'L':
			if(argc == 4){

				//capture user number
				if((user = atoi(argv[2])) < 1){
					perror("Error, invalid user");
					return -1;
				}

				//get key from args
				if((key = atoi(argv[3])) < 1){
					perror("Error assigning key");
					return -1;
				}

				//use key to find message queue id if queue exists
				if((msqid = msgget(key, 0400)) < 1){
					perror("Error accessing queue");
					printf("\n");
				}
				printf(".... Logged in as user: %lu\n", user);

				//print menu, only printing BACKDOOR option if user is 1
				if(user == 1){
				printf("Usage:\tread\n\twrite <receiverID> <msg>\n\tBACKDOOR\n\texit\n");
				}
				else{
				printf("Usage:\tread\n\twrite <receiverID> <msg>\n\texit\n");
		
				}
		
				//begin menu
				while(1){
		
					fgets(input, MSG_MAX, stdin);
		
					if(strlen(input) <= MAX_INPUT_LENGTH){
						//keep copy of original input			
						char inputCopy[strlen(input)];
						strcpy(inputCopy, input);
			
						//if input is exit, exit
						if(strcmp(input, exit) == 0){
							return 0;
						}
			
						//allocate memory for tokens
						tokenized = (char **)malloc(MAX_ARGS * sizeof(char*));
						for(int i = 0 ; i < MAX_ARGS ; i++){
							tokenized[i] = (char *)malloc(MAX_ARG_LENGTH * sizeof(char));
						}
			
						//tokenize input
						token = strtok(input, " ");
						while (token != NULL){
							strcpy(tokenized[j++], token);
							token = strtok(NULL, " ");
						}
						
						//new line for visual organization					
						printf("\n");			
	
	
	
	
						//******the first token dictates action to perform******
		
						//read option
						if(strcmp(tokenized[0], "read\n") == 0){			
			
							if( msgrcv(msqid, &mbuf, sizeof(mbuf)-sizeof(long), user, IPC_NOWAIT) < 0){
								perror("Error receiving message");
								printf("\n");
							}
							else{
							printf("From: %lu\t To: %lu\n", mbuf.senderID, mbuf.recipient );
							printf("Message: %s\n", mbuf.msg);
							}
						}
			
						//write option
						else if(strcmp(tokenized[0], "write") == 0){
							
							//get index for beginning of message to be sent from input
							//(once properly tokenized, the message begins at the third token)
							int inputLength = (strlen(tokenized[0]) + strlen(tokenized[1]));
	
							//copy inputCopy into itself starting from the index of the message
							//inputCopy now contains only the message segment
							strcpy(inputCopy, &inputCopy[inputLength+2]);				
			
							//populate message with recipient, sender and message
							mbuf.recipient = atoi(tokenized[1]); //recipient retreived from tokenized input
							mbuf.senderID = user;
							strncpy(mbuf.msg, inputCopy, MSG_MAX);
							buf_length = strlen(mbuf.msg) + 1;
			
							//send message to queue
							if(msgsnd(msqid, &mbuf, (sizeof(mbuf.msg) + 1), IPC_NOWAIT) < 0) 			
							{ 		
								perror("Error sending message");
								printf("\n");
							}
							else{
								printf("Message Sent:\n\tTo: %lu\n\tFrom: %lu\n\tMessage: %s\n", mbuf.recipient, mbuf.senderID, 	mbuf.msg);	
							}
						}
				
						//BACKDOOR option
						else if((strcmp(tokenized[0], "BACKDOOR\n") == 0) && user == 1){
				
							//retrieve the message queue information for given queue
							if((msgctl(msqid, IPC_STAT, &msgQueue)) < 0){
								perror("Retrive message queue into failed");
								printf("\n");
							}
							else{
								//for number of messages in queue..
								for(int i = 0 ; i < msgQueue.msg_qnum ; i++){
	
									//receive each message (0 for type argument receives next message of any type)				
									if( msgrcv(msqid, &mbuf, sizeof(mbuf)-sizeof(long), 0, IPC_NOWAIT) < 0){
										perror("Error receiving message");
									}
									else{	
										printf("\nFrom: %lu\t To: %lu\n", mbuf.senderID, mbuf.recipient);
										printf("Message: %s\n", mbuf.msg);
										
										//immediately resend message. queue remains identical given FIFO
										if(msgsnd(msqid, &mbuf, (sizeof(mbuf.msg) + 1), IPC_NOWAIT) < 0){ 		
											perror("Error sending message");
											printf("\n");
										}
									}					
								}	
							}
						}
						else{
							printf("Invalid Entry\n");
						}
					
		
		
						//free memory for tokens
	                	        	for(int i = (MAX_ARGS - 1); i >= 0; i--)
	                	        	{
	                	        	        free(tokenized[i]);
	                	        	}
						free(tokenized);
					}
					else{
						printf("\nError: Input is too long.\n");
					}
	
					//reprint menu, including BACKDOOR only if user is logged in as 1
					if(user == 1){
						printf("\nUsage:\tread\n\twrite <receiverID> <msg>\n\tBACKDOOR\n\texit\n");
					}
					else{
						printf("\nUsage:\tread\n\twrite <receiverID> <msg>\n\texit\n");
					}
				
					//reset j within while loop
					j=0;
				}
			}

			//invalid input
			else{
				printUsage();
				break;
			}
	
			//Delete a message queue
		case 'd':
		case 'D':

			//get key from args
			if((key = atoi(argv[2])) < 1){
				perror("Error assigning key");
				return -1;
			}

			//use key to find message queue id if queue exists
			if((msqid = msgget(key, 0400)) < 1){
				perror("Error accessing queue");
			}
			
			//delete corresponding message queue
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
	
