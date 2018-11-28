/* Blake Simmons
 * CIS 370
 * Lab 6 - Part 3
 * simmonsShell.c
 * October 20, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define SIZE 128
#define MAX_NUM_OF_ARGS 10


int main(int argc, char **argv)
{
 	char buf[SIZE];
	printf("\n>");
	
        //read until CTRL-C is pressed
        while(fgets(buf, SIZE, stdin))
        {

		pid_t pid;
	        int i, j = 0;
		char *token;
		buf[strlen(buf) - 1] = '\0';
		
		// create array for argument storage and allocate memory
		char **argumentArray;
		argumentArray = (char **)malloc(MAX_NUM_OF_ARGS * sizeof(char*));
		for(i = 0; i < MAX_NUM_OF_ARGS; i++)
		{
			argumentArray[i] = (char *)malloc(SIZE * sizeof(char));
		}

		//shell prompt
		printf("\n>");		

		//tokenize input and store in argumentArray
		token = strtok(buf, " ");
		while (token != NULL)
        	{
                        strcpy(argumentArray[j++], token);
			token = strtok(NULL, " ");
        	}
		
		argumentArray[j] = NULL;
		
		
		//create child process
		pid = fork();

        	if(pid < 0)
        	{
                	return -1;
        	}
        	else if(pid == 0)
        	{
			//child process executes program named in argument and receives any further arguments from the argumentArray
                	execvp(argumentArray[0], argumentArray);
			exit(1);
        	}
        	else
        	{
                	//wait until child process is done, then free malloc
			wait(NULL);
	                for( i = (MAX_NUM_OF_ARGS - 1); i>= 0; i--)
			{
                	        free(argumentArray[i]);
	                }

        	        free(argumentArray);

        	}


        }

	return 1;
}

