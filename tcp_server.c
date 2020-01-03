#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define DEFAULT_PORT_NUMBER 2100
#define MAX_LINE 4096

//socket test :server
int main(int arg,char ** argv){
//	int socket(int proto_family,int type,int protocol)//
//	proto_family:AF_INET(IPV4),AF_INET6(IPV6),AF_LOCAL
//	type:SOCK_STREAM,SOCK_DAGRAM,SOCK_RAW
//	protocol:IPPROTO_TCP,IPPROTO_UDP,

	int socket_fd,connect_fd;
	struct sockaddr_in server_address;
	char buffer[4096];
	int n;
	//initialize socket
	socket_fd=socket(AF_INET,SOCK_STREAM,0);
	if(socket_fd==-1){
		printf("creat socke error:%s(errno,%d)\n",strerror(errno),errno);
		exit(0);
	}
	//initialize socke address
	memset(&server_address,0,sizeof(server_address));
	server_address.sin_family=AF_INET;
	server_address.sin_addr.s_addr=htonl(INADDR_ANY);//set IP address to be any
	server_address.sin_port=htons(DEFAULT_PORT_NUMBER);
	//bind
	if(bind(socket_fd,(struct sockaddr*)&server_address,sizeof(server_address))==-1){
		printf("bind socket error:%s(errno,%d)\n",strerror(errno),errno);
		exit(0);	
	}
	//listen to connection
	if(listen(socket_fd,10)==-1){
		printf("listen socket error:%s(errorno,%d)\n",strerror(errno),errno);
		exit(0);
	}
	printf("============wait for client's requet============\n");
	while(1){
		connect_fd=accept(socket_fd,(struct sockaddr*)NULL,NULL);
		if(connect_fd==-1){
		printf("accept socket error:%s(errno,%d)\n",strerror(errno),errno);
		continue;	
		}
		//receive data from client 
		n=recv(connect_fd,buffer,MAX_LINE,0);
		if(!fork()){
			if(send(connect_fd,"hello,you are connected\n!",26,0)==-1)
			perror("send error\n");
			close(connect_fd);
			exit(0);
		buffer[n]='\0';
		printf("received message from client:%s\n",buffer);
		close(connect_fd);
		}
		
	}
	close(socket_fd);
		
}
