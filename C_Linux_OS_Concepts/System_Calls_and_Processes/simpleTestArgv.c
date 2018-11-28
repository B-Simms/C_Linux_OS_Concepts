/* Blake Simmons
 * CIS 370
 * Lab 5 - Part 3
 * simmonsTestArgv.c
 * October 18, 2018
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
	int i, sum1 = 0;
	int child1, child2, child3;
	float average, variance, sum2 = 0;
	
	//validate arguments
	if ( argc < MIN_ARGS ){
		printf("%s\n%s\n", "Error: Minimum 2 arguments.", 
			       "Usage: simmonsTestArgv <number> <number>");
		return -1;
	}
		
	
	//create first child process
	child1 = fork();

	//assure that the process used is a child of the main parent
	if(child1 > 0){
		printf("%s%d", "C1 > pid is ", child1);

		//compile the sum of arguments
		for(i = 1; i < argc; i++){
			sum1 += atoi(argv[i]);
		}
		
		//print results
		printf("%s%d\n", " Sum of Args: ", sum1);

		//create second child process
		child2 = fork();
		
		//assure that process used is child of main parent
		if(child2 > 0){
			printf("%s%d", "C2 > pid is ", child2);

			//compute average
			average = (float)sum1/(argc - 1);


			//print result
	                printf("%s%.2f\n", " Average of Args: ", average);

			//create third child process
			child3 = fork();

			//assure process is child of main parent
			if(child3 > 0){
				printf("%s%d", "C3 > pid is ", child3);

				//compute variance
				for (i = 1; i < argc; i++){
					sum2 = sum2 + pow((atoi(argv[i]) - average), 2);
				
				}
				variance = sum2 / (float)(argc - 1);
				
				//print result
				printf("%s%.2f\n", " Variance of Args: ", variance);
			}
			else{
				//wait for child 3 to end
				wait(NULL);
			}
		}
		else{

			//wait for child 2 to end
			wait(NULL);
		}

	}
	else{

		//wait for child 1 to end
		wait(NULL);
		return 0;
	}

}

