#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <string.h>

#define BLUE_TOOTH_BAUD_RATE 9600
#define BUFFER_SIZE_RECEIVE 1024
#define BUFFER_SIZE_SEND 512
int main(void){
	int 	fd;//file descriptor
	int 	intSendNumber=0;
	int 	intRcvNumber=0;
	char	strHello[]="Hello Jack,you are the best!\n";
	int 	index=0;
	char 	charRcvBuffer[BUFFER_SIZE_RECEIVE];
	char 	charSendBuffer[BUFFER_SIZE_SEND];
	//wiringPi setup
	wiringPiSetup();
	//open serial port
	fd=serialOpen("/dev/ttyAMA0",BLUE_TOOTH_BAUD_RATE);
	if(fd<0) {
		printf("fail to open serial port");		
		return -1;
	}
	printf("serial port opened,waiting for your message...\n");
	serialPuts(fd,strHello);
	while(1){	
		//serialPuts(fd,strHello);
		intRcvNumber=serialDataAvail(fd);
		if(intRcvNumber>0){
			printf("received message %d bytes\n",intRcvNumber);
			for(index=0;index<intRcvNumber;index++)
				charRcvBuffer[index]=serialGetchar(fd);
			charRcvBuffer[index]='\0';
			printf("received message:%s\n",charRcvBuffer);
			//charRcvBuffer[index]='\n';
			//empty receive buffer in serail port
			serialFlush(fd);
			//printf("received message: %s",charRcvBuffer);
			//empty receive buffer in program
			/*for(index=0;index<BUFFER_SIZE_RECEIVE;index++)
				charRcvBuffer[index]='\0';*/
		}
		//time delay
		delay(100);
		//empty receive buffer
		
		//string compare
		if(strcmp(charRcvBuffer,"quit")==0) break;	
	}
	serialClose(fd);
	printf("serail port closed\n");
	return 0;
}

