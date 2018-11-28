/* Blake Simmons
 * CIS 370
 * Lab 7 - Part 4
 * simmonsCP.c
 * October 28, 2018
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE 1024
#define MIN_ARGS 3

int main(int argc, char **argv){
	
	//create necessary variables
	int pipefd[2];
	pid_t child;
	ssize_t nread;
	char buf[BUFSIZE];
	int fd, filedest;
	
	//create pipe
	pipe(pipefd);

	//validate arguments
	if ( argc !=  MIN_ARGS ){
		printf("%s\n%s\n", "Error: Please define a file and copy location.", 
			       "Usage: simmonsCP <file to copy> <destination file>");
		return -1;
	}
		
	
	//create child process
	child = fork();


	//assure that the process used is a child of the main parent
	if(child == 0){
		//close write end of pipe
		close(pipefd[1]);

	        //open destination file if it exists, otherwise create
        	filedest = open(argv[2], O_WRONLY | O_CREAT, 0644);
	
		//read data from read end of pipe and write to file
	        while( (nread = read(pipefd[0], buf, BUFSIZE)) > 0){
                	if( write(filedest, buf, nread) < nread){
                        	close(fd);
			}
                }
	
		//close file, read end of pipe and fd
		close(fd);
		close(filedest);
		close(pipefd[0]);
		exit(1);
		
	}
			
	else{

		//close read end of pipe in parent
		close(pipefd[0]);

	        //if input file exists, open the file
        	if((open(argv[1], O_RDONLY) == -1)){
                	printf("%s%s\n", "Error opening file", argv[1]);
       		 }	
        	else{
             		fd = open(argv[1], O_RDONLY);
        	}

		//read from file and write to write end of pipe
        	while( (nread = read(fd, buf, BUFSIZE)) > 0){
             		if( write(pipefd[1], buf, nread) < nread){
                   	close(fd);
             		}
		}
		//close write end of pipe
		close(pipefd[1]);

		//wait for child to finish
		wait(NULL);
		

		return 0;
	}
	
}

