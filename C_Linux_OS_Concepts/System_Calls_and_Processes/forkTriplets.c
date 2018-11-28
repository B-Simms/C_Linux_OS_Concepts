/* Blake Simmons
 * CIS 370
 * Lab 5 - Part 2
 * simmonsTriplets.c
 * October 18, 2018
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#define COUNT 3

int main(){
	
	//declare necessary variables
	int i, status;
	pid_t child, waitpid;

	//create for loop in order to create a distinct child each iteration
	for(i = 1; i <= COUNT; i++ ){

		//create and determine child process
		if(child = fork() == 0){
		
		//print child information and its parent information
		printf("%s%d%s%d%s%d\n", "This is the child process ", i, " with pid: ", getpid(), " and ppid: ", getppid());
                sleep(5);
			if(i=3){
				exit(0);
			}
		}
	}

	//parent waits for child to finish
        while((waitpid = wait(&status)) > 0); 
	
	printf("%s%d%s%d%s\n", "This is the parent process with pid: ", getpid(), " and ppid: ", getppid(), ". All my child processes are done.");
 
	
}



