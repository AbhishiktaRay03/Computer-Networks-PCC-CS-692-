#include "chatcustom.h"

void* sender(void* args) {
    char buf[MAX];
    while(chat_active) {
        fgets(buf, MAX, stdin);
        int length = sizeof(caddr);
        sendto(ss, buf, strlen(buf) + 1, 0, (struct sockaddr*)&caddr, length);
        if(!strcmp("stop\n", buf)) {
            chat_active = false;
            pthread_exit(NULL);
        }
    }
}

void* receiver(void* args) {
    char buf[MAX];
    while(chat_active) {
        int length = sizeof(caddr);
        recvfrom(ss, buf, MAX, 0, (struct sockaddr*)&caddr, &length);
        printf("\nUser 2: %s", buf);
        if(!strcmp("stop", buf)) {
            chat_active = false;
            pthread_exit(NULL);
        }
    }
}

void main() {
    pthread_t t1, t2;
    ss = socket(AF_INET, SOCK_DGRAM, 0);
    struct sockaddr_in saddr, daddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = inet_addr(ip);
    saddr.sin_port = htons(S_PORT_NO);
    daddr.sin_family = AF_INET;
    daddr.sin_addr.s_addr = inet_addr(ip);
    daddr.sin_port = htons(D_PORT_NO);
    bind(ss, (struct sockaddr*)&saddr, sizeof(saddr));
    caddr = daddr;
    pthread_create(&t1, NULL, sender, NULL);
    pthread_create(&t2, NULL, receiver, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Chat terminated...\n");
}
