/*Blake Simmons
  CIS 370
  Lab - 9 - message.h
  November 13, 2018
*/

#ifndef MESSAGE_BUFFER
#define MESSAGE_BUFFER

#define MSG_MAX 4056

typedef struct messageBuffer
{
	long recipient; //recipient of message
	long senderID; //id number of the sender	
	char msg[MSG_MAX]; //content of the message
}messageBuffer;

#endif
