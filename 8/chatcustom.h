#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
#include<pthread.h>

#define S_PORT_NO 7000
#define D_PORT_NO 7001
#define MAX 100
#define ip "127.0.0.1"

int ss;
bool chat_active=true;
struct sockaddr_in caddr;
