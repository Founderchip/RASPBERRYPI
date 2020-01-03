#include<errno.h>
#include<string.h>
#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <string.h>
/*
this function is used to config HC05 at AT mode
Jack Lee
2019-12-27
*/
#define BAUD_RATE_AT 38400
#define BAUD_RATE_AT_9600 9600
#define BUFFER_SIZE_RECEIVE 1024
#define BUFFER_SIZE_CMD 50
int main(void){
	int 	fd;//file descriptor
	int 	intCharNumReceived;//receive char/byte number
	char 	charAT_Cmd[BUFFER_SIZE_CMD];
	char 	index=0;
	char	charConfirm='n';
	int	intLenCmd=0;
	char 	charBufferRcv[BUFFER_SIZE_RECEIVE];
	//initialize wriingPi
	wiringPiSetup();
	//baud rate for AT mode shoud be 38400bps
	fd=serialOpen("/dev/ttyAMA0",BAUD_RATE_AT);
	//fd=serialOpen("/dev/ttyAMA0",BAUD_RATE_AT_9600);
	if(fd==-1) {
		printf("fail to open serial port");		
		perror("serialOpen");//print error information,this function is included in <stdio.h>
		//exit(EXIT_FAILURE);
		return 0;
	}
	printf("serial port opened.\n");
	printf("please input the AT command:\n");
	while(1){
		scanf("%s %c",charAT_Cmd,&charConfirm);//receiv AT command
		printf("you entered command :%s\n",charAT_Cmd);
		//get the length of input command
		intLenCmd=strlen(charAT_Cmd);
		//AT command for HC05 must be terminated by '\r\n';
		//add '\r\n' for AT command
		charAT_Cmd[intLenCmd]='\r';//carriage return 
		charAT_Cmd[intLenCmd+1]='\n';//change line
		charAT_Cmd[intLenCmd+2]='\0';//null terminal
		printf("Are you sure to send this command to HC05 blue module?\n");
		printf("[y]=yes;[n]=no;[q]=stop and quit\n");	
		//you can input [q] to quit the process
		if(charConfirm=='q'||charConfirm=='Q') break;
		//input [y] to send to AT command;
		if(charConfirm=='y'||charConfirm=='Y'){
			//serialPrintf(fd,charAT_Cmd);
			serialPuts(fd,charAT_Cmd);//send null-terminated string
			delay(150);
			printf("Have sent AT command:%s\n",charAT_Cmd);
		}
		//get char number availber
		intCharNumReceived=serialDataAvail(fd);
		if(intCharNumReceived>0 && intCharNumReceived<BUFFER_SIZE_RECEIVE){
			//if char nubmer received larger than 0
			for(index=0;index<intCharNumReceived;index++)
				charBufferRcv[index]=serialGetchar(fd);	
			charBufferRcv[index]='\0';//string end	
			delay(100);
			serialFlush(fd);//delete serail receive buffer			
		//print information
		printf("serial port received %d chars\n",intCharNumReceived);
		printf("received:%s\n",charBufferRcv);
		}
	}	
	serialClose(fd);//close serial port
	printf("serial port closed\n");
	return 1;
}

