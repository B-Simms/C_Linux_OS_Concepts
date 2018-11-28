/* Blake Simmons
 * CIS 370
 * Lab 6 - Part 2
 * simmonsForkExec.c
 * October 18, 2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int ForkExec(char **temp)
{
	pid_t pid;
	
	pid = fork();

	if(pid < 0)
	{
		return -1;
	} 
	else if(pid == 0)
	{
		printf("%s%s\n\n", 
		       "Executing ", temp[1]);
		execvp(temp[1], temp + 1);
		exit(1);
	}
	else
	{
		wait(NULL);
		return 1;
	}
	
}

int main(int argc, char **argv)
{
	ForkExec(argv);
	return 1;
}

