#ifndef BUFFER
#define BUFFER

#define MAX_SIZE 8

typedef struct bufferStruct {
		int in;
		int out;
		int content[MAX_SIZE];
}bufferStruct;

key_t someKey = 1111;

#define memorySize sizeof(bufferStruct)
#endif
