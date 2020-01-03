#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <string.h>

#define BLUE_TOOTH_BAUD_RATE 9600
#define BUFFER_SIZE_RECEIVE 1024
#define BUFFER_SIZE_SEND 512
#define SIZE_INFORMATION 50
int main(void){
	int 	fd;//file descriptor
	int 	intSendNumber=0;
	int 	intRcvNumber=0;
	char	strHello[SIZE_INFORMATION];
	int 	index=0;
	char 	charRcvBuffer[BUFFER_SIZE_RECEIVE];
	char 	charSendBuffer[BUFFER_SIZE_SEND];
	//initialize hello array
	strcpy(strHello,"hello jack,you are the best!\n");
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
			//make null terminal
			charRcvBuffer[index]='\0';
			printf("received message:%s\n",charRcvBuffer);
			//serialPrintf(fd,"I have received following message:\n");
			strcpy(strHello,"I have received following messages:\n");
			serialPuts(fd,strHello);
			delay(100);
			serialPuts(fd,charRcvBuffer);
			delay(150);
			//send back by blue tooth
			//serialPrintf(fd,charRcvBuffer);
			//empty receive buffer in serail port
			serialFlush(fd);
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

