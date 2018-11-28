/* Blake Simmons
 * CIS 370
 * Lab 8 - Part 3
 * simmonsConsumer.c
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

	//program runs until user enters 0
	while(getchar() != '0'){

		//if buffer is empty, do not consume any input
		if(buffer->out == buffer->in){
			printf("Buffer is empty.\n");
		}

		//if buffer is not empty...
		else{
			//consume input (replace contents with null value)
			printf("%d consumed.\n", buffer->content[buffer->out]);
			buffer->content[buffer->out] = NULL;
			
			//increment out position			
			buffer->out = (buffer->out + 1) % MAX_SIZE;
	
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
