/*  ESD-Final Project library - client.c
    SUBIR KUMAR PADHEE
    SHRIVATHSA KESHAVA MURTHY
    ECEN-5613
*/
/*Author 		: Subir Kumar Padhee, Shrivathsa Murthy
  Date 			: 20/4/16
  Objective 	: client Arduino Yun program to parse Intel Hex file and create 
					sockets to communicate with the 8051
					
  bug reports? write to shmu7023@colorado.edu, supa2799@colorado.edu
  
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <math.h>

#define NUM0 0x30
#define NUM7 0x37
#define NUM9 0x39

#define ALPHA_F 0x46

/*   Routine Description:
//   Function name : myAtoi - converts 2 unsigned chars into an integer
//   Arguments	 : char * - 2 hex digits
//   Return 	 : int - converted integer value 
*/

int 
myAtoi(char *inputBufferArray){
  int inputBuffer=0,i;

  // loop through 2 times as there are 2 chars
  for(i=0;i<2;i++){
    unsigned char temp;
    if ( inputBufferArray[i] > ALPHA_F){
      temp = inputBufferArray[i]-0x57;
      }
    else if ( inputBufferArray[i] > NUM9){
      temp = inputBufferArray[i]-NUM7;
      }
    else{
      temp = inputBufferArray[i]-NUM0;
      }

    // convert input characters to integer
    inputBuffer = inputBuffer+(temp*pow(16,2-i-1));
    }
    return inputBuffer;
}



/* Routine Description: main function

 * Parses hex file and Connects to HTPP remote server (192.168.0.3)on port 5000 to send the contents
 * Implements Chat client server application as well
 * 
*/

int
main(int argc , char **argv )
{
  FILE *filePointer;
  
  char setting[25];
  char value[100],value1[100];
  char portNum[5][6];
  int i=0;
  char *returnValue;
  struct sockaddr_in sin;
  struct hostent *hp;
  int s;
  char buffer[200]="\0",temp[200]="\0";
  int length,opcodeLength=0;
  char opcodes[10000]="\0";
  char lengthChar[3];
        

  if (  argc == 1){
	// if no args, it is programmer mode
 	// read the file in the PWD
  	filePointer = fopen("upload.hex","r");
        if ( filePointer == 0){
                perror("ERROR in Opening file! Input file doesn't exist");
                exit(1);
        }

  	lengthChar[2]='\0';
        while(fgets(buffer,sizeof(buffer),filePointer)) {
        //printf("Entering while\n");
        if(strlen(buffer) <= 1 )                              /* if it's an empty line, continue */
                continue;

        printf("%s",buffer);

        lengthChar[0]=buffer[1]; lengthChar[1]=buffer[2];
        length = myAtoi(lengthChar); // convert to hex
        if ( length == 0){
                continue;
        }
        strncpy(temp,&buffer[9],length*2);
        temp[2*length]='\0';
        printf("%s\n",temp);
        strcat(opcodes,temp);
        opcodeLength += length;
        
        }
        opcodes[strlen(opcodes)]='\0';
        printf("***********Parsed opcodes = %d bytes*******\n%s\n",opcodeLength,opcodes);

   }
  // we have the parsed file at this point
  // convert destn ip to an address

  hp = gethostbyname("192.168.0.3");
  if( !hp){
    fprintf(stderr, "Unknown Host : \n");
    exit(1);
    }

  //create address
  bzero( (char*)&sin, sizeof(sin) );
  sin.sin_family = AF_INET;
  bcopy(hp->h_addr,(char*)&sin.sin_addr, hp->h_length );
    
  // we have address at this point

    if ((sin.sin_port=htons((unsigned short)atoi("5000" ))) == 0){
      printf("can't get  port number\n");
      exit(1);
      }

  // create a new socket
  if ( ( s = socket( AF_INET, SOCK_STREAM, 0 )) <0 ){
      printf("Error creating a socket\n");
      exit(1);
      }

  // connect socket to 8051
  if (  connect( s, (struct sockaddr *)&sin, sizeof(sin) ) < 0 ){
      printf("Server listening on Port number is dead\n");
      exit(1);
      }

  printf("Connected to 8051 server\n");
  // argc = 1 means no external command line args. i.e. programmer mode
  // if that's the case, send the opcodes to server , close socket and return
  if ( argc == 1 ){
     opcodes[9999] = '\0';
     printf("Sent this \n%s\n",opcodes);
     send(s,opcodes,strlen(opcodes)+1,0);
     close(s);
     return 0;
     }

  // If it comes here, it's a 
  // chat server
    
  printf("\nEntering 8051 TCP chat client\n\nMe:	 LET'S TALK\n");

  // Send '*' so that 8051 identifies we are in CHAT mode

  send(s,"*",2,0);
    
  char sendBuf[100];
  char buff[200];
  int n=0;
  while(1){
	// empty it to be on the safer side
    	strcpy(sendBuf,"");
    	strcpy(buff,"");
    	
	// receive response from server

    	n=recv(s,buff,sizeof buff, 0);
    	buff[n]='\0';
    	printf("\n8051    :		%s\n",buff);

	// if message == "bye" close session
    	if(!strncmp(buff,"bye",3)){
    		printf("\n	Exit from chat client\n");
    		break;
    	}
    	printf("\nMe      :		");
    	// o else, get input from user here and send it to 8051. this process continues

    	fgets ( sendBuf, sizeof(sendBuf), stdin );
    	sendBuf[99] = '\0';
    	printf("\n");
    	send(s,sendBuf,strlen(sendBuf)+1,0);
    	//printf("Waiting..");
    	if ( !strncmp(sendBuf,"bye",3)){
    	  printf("\n      Exit from chat client\n");
          break;
          }	                        
    	}

    close(s);
    return 0;

}



