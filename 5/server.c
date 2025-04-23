#include "custom.h"

char* bit_stuffing(char* input){
	char* output=(char*)malloc(MAX*sizeof(char));
	int i,j=0,consecutive=0;
	int flag=0;
	for(i=0;i<strlen(input);i++){
		output[j++]=input[i];
		if(input[i]=='1'){
			consecutive++;
			if(consecutive==5 && flag){
				output[j++]='0';
				consecutive=0;
				flag=0;
			}
		}
		else {
			consecutive=0;
			flag=1;
		}
	}
	output[j]='\0';
	return output;
}

int main(){
	//1. declarations
	char buffer[MAX];
	int server_fd,client_fd;
	struct sockaddr_in server_addr, client_addr;
	//2. socket creation
	server_fd=socket(AF_INET,SOCK_STREAM,0);
	if(server_fd<0){
		perror("Socket creation failed\n");
		exit(0);
	}
	//3. length and address initialization
	int server_len=sizeof(server_addr);
	int client_len=sizeof(client_addr);
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
	server_addr.sin_port=htons(PORT_NO);
	//4. memset
	memset(&buffer,0,sizeof(buffer));
	//5. bind
	if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
		perror("Bind failed\n");
		exit(0);
	}
	//6. listen
	if(listen(server_fd,1)<0){
		perror("Listen failed\n");
		exit(0);
	}
	//7. accept
	client_fd=accept(server_fd,(struct sockaddr*)&client_addr,&client_len);
	if(client_fd < 0){
		perror("Accept failed\n");
		exit(0);
	}
	//8. communication
	while(1){
		//a. read
		if(read(client_fd,buffer,sizeof(buffer))<0){
			perror("Read failed\n");
			exit(0);
		}
		//b. print
		printf("Data received is: %s\n",buffer);
		//c. compare
		if(!strncmp(buffer,"exit",4)){
			printf("Program terminated.\n");
			break;
		}
		//d. bit stuffing
		char modified[MAX];
		char *ptr = bit_stuffing(buffer);
		strcpy(modified, ptr);
		free(ptr);
		//e. write
		if(write(client_fd,modified,strlen(modified))<0){
			perror("Write failed\n");
			exit(0);
		}
		//f. memset
		memset(&buffer,0,sizeof(buffer));
	}
	//9. close
	close(client_fd);
	close(server_fd);
	return 0;
	
	
}
