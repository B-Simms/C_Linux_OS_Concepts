/* Blake Simmons
 * CIS 370
 * Lab 7 - Part 5
 * simmonsDUP.c
 * October 28, 2018
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define BUFSIZE 256

int main(int argc, char **argv){
	
	int index = 1;
	unsigned char flag;
	int outFD, oldFD;
	char string[BUFSIZE];
	oldFD = dup(1);
	
	//validate number of arguments 
	if(argc != 2){

		printf("%s\n", "Error! Program requires an argument.");
		printf("%s\n", "Usage: simmonsDUP <destination file>");
		exit(1);
	}//if(argc != 2)
	
	//open output file
	if(open(argv[1], O_WRONLY | O_CREAT, 0644) < 0){
		printf("%s\n", "Error opening file.");
		exit(1);
	}
	else{
		outFD = open(argv[1], O_WRONLY | O_CREAT, 0644);
	}
	//program loops while the user input is not char '0'

	scanf(" %c", &flag);
	while(flag != '0'){
		//printf("%s%c\n", "Flag is ", flag);
		
		//if input is 'f', out put switches to file
		if(flag == 'f'){
			dup2(outFD, 1);
			
		}//if(flag == 'f')
		
		//if input is 's' output switches to screen
		else if(flag == 's'){
			dup2(oldFD, 1);	
	
		}//else if(flag == 's')
		
		//if input is neither 'f', 's', or '0', error message is displayed.
		else{
			dup2(oldFD, 1);
			printf("%s\n", "Error: You must enter f, s, or 0");
		}

		sprintf(string, "The Index Is %d\n", index);
		write(1, &string, strlen(string));	
		index++;	
		scanf(" %c", &flag);

	}//while(flag != 0)
	
	//close output file and saved dup()
	close(outFD);
	close(oldFD);	

}//int main

