#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <math.h>

#define MAX 100
int code[32];
char res[MAX];

int ham_calc(int position, int c_l) {
	int count = 0, i,j;
	i = position - 1;
	
	while(i < c_l) {
		for(j=i; j< i + position; j++) {
			if(code[j] == 1) {
				count++;
			}
		}
		i = i+ 2*position;
	}
	if(count % 2 == 0) return 0;
	else return 1;
}

char *reverse(char *data) {
	char *rev=(char*)malloc((strlen(data)+1)*sizeof(char));
	int k=0;
	for(int i = strlen(data)-1; i>= 0; i--) {
		rev[k]=data[i];
		k++;
	}
	rev[k] = '\0';
	printf("Reverse is : %s\n",rev);
	return rev;
}

void solve(char *data) {
	int n = strlen(data);
	int i,p_n=0,c_l,j,k;
	i=0;
	char *temp = reverse(data);
	strcpy(data, temp);
	free(temp);
	while(n > (int)pow(2,i)-(i+1)) {
		p_n++;
		i++;
	}
	c_l=p_n + n;
	j=k=0;
	for(i=0; i<c_l; i++) {
		if(i == ((int)pow(2,k) - 1)) {
			code[i] = 0;
			k++;
		}
		else {
			code[i] = data[j] - '0';
			j++;
		}
	}
	
	for(i = 0; i< p_n; i++) {
		int position = (int)pow(2,i);
		int value = ham_calc(position, c_l);
		code[position - 1] = value;
 	}
 	printf("\nThe Generated Hamming Code word is: ");
 	for(i = 0; i< c_l; i++) {
 		printf("%d",code[i]);
 		res[i] = (code[c_l - i - 1]==0) ? '0':'1';
 	}
 	res[i]='\0';
 	printf("\n");
}

int main() {
	int server_fd, server_len, client_len, checkres;
	struct sockaddr_in server_address, client_address;
	char input[MAX], result[MAX];
	
	server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(8888);
	server_len = sizeof(server_address);
	bind(server_fd, (struct sockaddr*)&server_address, server_len);
	printf("Server is Running...\n");
	
	client_len = sizeof(client_address);
	checkres = recvfrom(server_fd, input, MAX, 0, (struct sockaddr*)&client_address, &client_len);
	if(checkres!=-1) {
		printf("Client Connected...\n");
		printf("Server Received: %s\n",input);
		strcpy(res,"Hello from Server!");
		sendto(server_fd, res, strlen(res)+1, 0, (struct sockaddr*)&client_address, client_len);
		strcpy(res, "");
		strcpy(input, "");
	}
	else {
		printf("Client Disconnected...\n");
		exit(1);
	}
	
	while(1) {
		recvfrom(server_fd, input, MAX, 0, (struct sockaddr*)&client_address, &client_len);
		printf("Server Received Dataword: %s\n", input);
		if(strcmp(input, "end")==0) {
			break;
		}
		solve(input);
		
		strcpy(result, res);
		strcpy(res, "");
		printf("Sending codeword %s to the Client.\n", result);
		sendto(server_fd, result, strlen(result)+1, 0, (struct sockaddr*)&client_address, client_len);
	}
	close(server_fd);
	return 0;
}







