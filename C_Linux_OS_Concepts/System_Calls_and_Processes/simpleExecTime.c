/* Blake Simmons
 * CIS 370
 * Lab 5 - Part 2
 * simmonsTime.c
 * October 18, 2018
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>

int main(int argc, char **argv){

	// validate arguments
	if(argc == 2){
		
		//create structs
		struct timeval startTime, endTime;

		//get start time
		gettimeofday(&startTime, NULL);

		//create and determine child process
		if(fork() == 0){
			execlp(argv[1], argv[1], NULL); //replace child process with ps
		exit(1);
		}
		
		//parent process waits for child to finish
		else{
			
			wait(NULL);
			
			//get end time
			gettimeofday(&endTime, NULL);

			//calculate duration
			long duration = endTime.tv_usec - startTime.tv_usec;

			//print results
			printf("\nElapsed time: :%ld usec\n", duration);
		}
	}
	else{
		//error
		printf("%s\n", "ERROR: Invalid arguments");
		printf("%s\n", "Usage: simpleExecTime <name of program>, Only one argument permitted.");
	}
	
	return 0;

}

