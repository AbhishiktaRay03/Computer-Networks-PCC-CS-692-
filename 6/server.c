#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define key 1234
#define port 4444
#define size 50
#define ip "127.0.0.1"
#define backlog 5

typedef struct Clients{
	int client_fd, server_fd;
	int i;
}client;

void handler(int signum){
	if(signum == SIGINT){
		printf("\nServer is terminating\n");
		signal(SIGINT, SIG_DFL);
		kill(getpid(), SIGINT);
	}
}

int isValid(char *str){
	signal(SIGINT, handler);
    int i, j, count=0, n = strlen(str), sum=0;

    if(str[0] == '.' || str[n-1] == '.')
        return 0;
    for(i=0; i<n; i++){
        if(!isdigit(str[i]) && str[i]!='.'){
            //puts("Condition - 1");
            return 0;
        }
        else if(isdigit(str[i])){
            //puts("Condition - 2");
            sum = sum * 10 + (str[i]-'0');
        }
        else{
            //puts("Condition - 3");
            if(str[i+1] == '.')
                return 0;
            count++;
            if(count > 3)
                return 0;
            else if(sum > 255)
                return 0;
            sum = 0;

        }
    }
    if(count != 3 || sum > 255)
        return 0;
    return 1;
}

void *check(void *args){
	client c = *(client*)args;
	char msg[size];
	int ans;
	while(1){
		read(c.client_fd, msg, sizeof(msg));
        if(!strcmp(msg, "0000"))
            break;
        printf("Client - %d\n",c.i);
        printf("Received data : %s\n",msg);

        ans = isValid(msg);
        if(ans)
            strcpy(msg, "Valid ip address!");
        else 
            strcpy(msg, "Invalid ip address!");

        puts(msg);
        write(c.client_fd, msg, sizeof(msg));
        
        puts("");
	}
	c.i--;
	pthread_exit(NULL);
}

void main(){
    struct sockaddr_in addr, caddr;
    int server_fd, client_fd, caddr_len;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(4444);

    bind(server_fd, (struct sockaddr *) &addr, sizeof(addr));
    listen(server_fd, backlog);

    caddr_len = sizeof(caddr);
	
	pthread_t th;
	client c;
	c.server_fd = server_fd;
	c.i = 0;
    while(1){
        printf("Server is waiting\n\n");
		client_fd = accept(server_fd, (struct sockaddr *)&addr, &caddr_len);
		c.client_fd = client_fd;
		pthread_create(&th, NULL, (void*)check, (void *)&c);
        printf("Created thread : %d\n",++c.i);        
    }
    printf("Server is terminating\n");
    usleep(1000);
    close(server_fd);
}
