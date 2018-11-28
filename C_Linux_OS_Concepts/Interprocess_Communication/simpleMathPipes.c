/* Blake Simmons
 * CIS 370
 * Lab 7 - Part 3
 * simmonsPart3.c
 * October 28, 2018
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <math.h>

#define MIN_ARGS 2

int main(int argc, char **argv){
	
	//create necessary variables
	int pipefd[2], pipefd2[2];
	int i, sum = 0;
	pid_t child1, child2, child3;
	float average, variance, sum2 = 0;
	
	//create two pipes
	pipe(pipefd);
	pipe(pipefd2);


	//validate arguments
	if ( argc < MIN_ARGS ){
		printf("%s\n%s\n", "Error: Minimum 2 arguments.", 
			       "Usage: simpleMathPipes <number> <number>");
		return -1;
	}
		
	
	//create first child process
	child1 = fork();

	//assure that the process used is a child of the main parent
	if(child1 == 0){
		//close read end of pipefd and entirety of pipefd2
		close(pipefd[0]);
		close(pipefd2[0]);
		close(pipefd2[1]);

		printf("%s%d", "C1 > pid is ", child1);

		//compile the sum of arguments
		for(i = 1; i < argc; i++){
			sum += atoi(argv[i]);
		}
	
		//print results
		printf("%s%d\n", " Sum of Args: ", sum);

		//write the sum to write end of pipefd
		write(pipefd[1], &sum, sizeof(int));		

		//close pipefd
		close(pipefd[1]);
		exit(1);
	}


	//create second child process
	child2 = fork();

	//assure that process used is child of main parent		
	if(child2 == 0){
		int sum1;
		//sleep(1);

		//close write end of pipefd and read end of pipefd2
		close(pipefd[1]);
		close(pipefd2[0]);

		//read from read end of pipefd
		sum1 = read(pipefd[0], &sum, sizeof(int));
		printf("%s%d", "C2 > pid is ", child2);

		//compute average
		average = (float)sum/(argc - 1);


		//print result
                printf("%s%.2f\n", " Average of Args: ", average);

		//write average to write end of pipefd2
		write(pipefd2[1], &average, sizeof(float));

		//close read end of pipefd and write end of pipefd2
		close(pipefd[0]);
		close(pipefd2[1]);
		exit(1);

	}

	//create third child process
	child3 = fork();

	//assure process is child of main parent
	if(child3 == 0){
		float average1;
		//sleep(2);

		//close entirety of pipefd and write end of pipefd2
		close(pipefd[0]);
		close(pipefd[1]);
		close(pipefd2[1]);

		printf("%s%d", "C3 > pid is ", child3);
		
		//read average from pipefd2		
		average1 = read(pipefd2[0], &average, sizeof(float));
	
		//compute variance
		for (i = 1; i < argc; i++){
			sum2 = sum2 + pow((atoi(argv[i]) - average), 2);
		}

		variance = sum2 / (float)(argc - 1);
				
		//print result
		printf("%s%.2f\n", " Variance of Args: ", variance);
	
		//close read end of pipefd2
		close(pipefd2[0]);
		exit(0);
	}
			
	else{

		//close all pipes in parent process and wait for children to end
		close(pipefd[0]);
		close(pipefd[1]);
		close(pipefd2[0]);
		close(pipefd[1]);

		wait(NULL);
		wait(NULL);
		wait(NULL);
		return 0;
	}

}

