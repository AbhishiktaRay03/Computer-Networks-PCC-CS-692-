#include "custom.h"

int main(){
	//1. declarations
	char buffer[MAX];
	int client_fd;
	struct sockaddr_in client_addr;
	//2. socket creation
	client_fd=socket(AF_INET,SOCK_STREAM,0);
	if(client_fd<0){
		perror("Socket creation failed\n");
		exit(0);
	}
	//3. length and address initialization
	int client_len=sizeof(client_addr);
	client_addr.sin_family=AF_INET;
	client_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	client_addr.sin_port=htons(PORT_NO);
	//4. memset
	memset(&buffer,0,sizeof(buffer));
	//5. connect
	if(connect(client_fd,(struct sockaddr*)&client_addr,sizeof(client_addr))<0){
		perror("Connect failed\n");
		exit(0);
	}
	//6. communication
	while(1){
		//a. input
		printf("Enter the msg to be sent: \n");
		scanf("%s",buffer);
		//b. write
		if(write(client_fd,buffer,strlen(buffer))<0){
			perror("Write failed\n");
			exit(0);
		}
		//c. compare
		if(!strncmp(buffer,"exit",4)){
			printf("Program terminated.\n");
			break;
		}
		//d. read
		int m = read(client_fd,buffer,sizeof(buffer));
		if(m<0){
			perror("Read failed\n");
			exit(0);
		}
		//e. print and null terminate
		buffer[m]='\0';
		printf("Data received is: %s\n",buffer);
		//f. memset
		memset(&buffer,0,sizeof(buffer));
	}
	//7. close
	close(client_fd);
	return 0;
	
	
}
