/* Blake Simmons
 * CIS 370
 * Lab 8 - Part 3
 * simmonsProducer.c
 * November 6, 2018
 */


#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

//buffer.h contains bufferStruct definition, as well as key and MAX_SIZE values
#include "buffer.h"

int main(){

	//declare variables and buffer struct	
	int id, i;
	struct bufferStruct *buffer;

	//create shared memory location
	if( (id = shmget(someKey, memorySize, IPC_CREAT | 0666)) < 0 ){
		perror("Error creating shared memory\n");
		exit(-1);	
	}

	//attach the buffer struct to shared memory  
	if((buffer = (struct bufferStruct *) shmat(id, 0, 0)) <= (struct bufferStruct *)(0)){
		perror("Error attaching shared memory\n");
		exit(-1);
	}

	//set default positions of in and out indices
	buffer->in = 0;
	buffer->out = 0;
	
	//program runs until user enters 0
	while(getchar() != '0'){

		//if buffer is full, do not produce more input
		if(((buffer->in + 1) % MAX_SIZE) == buffer->out){
			printf("Buffer is full.\n");
		}

		//generate input
		else{
			buffer->content[buffer->in] = rand() % (99 + 1 - 1) + 1;
			printf("%d produced.\n", buffer->content[buffer->in]);
			
			//increment in position
			buffer->in = ((buffer->in + 1) % MAX_SIZE);
			
			//print buffer contents
			for (i = 0; i < MAX_SIZE ; i++){
				if(buffer->content[i] != 0){
					printf("%d ", buffer->content[i]);
				}
			}
			printf("\n");
			fflush(stdin);
		}	
	}

	//detach buffer from shared memory
	shmdt(buffer);

	//remove shared memory
	shmctl(id, IPC_RMID, NULL);	
	return 0;
}
