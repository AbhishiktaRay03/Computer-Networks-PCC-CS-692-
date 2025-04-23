#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX 100
#define PORT_NO 8081
#define IP "224.0.0.1"

int sockfd, reuse = 1;
struct sockaddr_in multicast;
socklen_t multilen;
bool flag = false;
char buff[MAX];

void *receive(void *args) {
    while (true) {
        int n = recvfrom(sockfd, buff, MAX, 0, NULL, NULL);
        if (n < 0) {
            perror("recvfrom");
            continue;
        }

        // Check for termination signal
        if (strcmp(buff, "end") == 0) {
            printf("\nReceived termination signal. Exiting...\n");
            close(sockfd);
            exit(0);
        }

        if (!flag) {
            printf("Received: %s\n", buff);
        }
        flag = false;
    }
    pthread_exit(NULL);
}

void *sender(void *args) {
    while (true) {
        printf("You: ");
        scanf(" %[^\n]", buff);
        getchar();
        flag = true;

        if (sendto(sockfd, buff, strlen(buff) + 1, 0, (struct sockaddr*)&multicast, multilen) < 0) {
            perror("sendto");
        }

        // If sender wants to end, also exit locally
        if (strcmp(buff, "end") == 0) {
            printf("Sent termination signal. Exiting...\n");
            close(sockfd);
            exit(0);
        }
    }
    pthread_exit(NULL);
}

int main() {
    struct ip_mreq req;
    pthread_t rthread, sthread;

    printf("Multicast Chat\nType 'end' to exit all clients.\n");

    multicast.sin_family = AF_INET;
    multicast.sin_addr.s_addr = inet_addr(IP);
    multicast.sin_port = htons(PORT_NO);
    multilen = sizeof(multicast);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt reuse");
        exit(EXIT_FAILURE);
    }

    if (bind(sockfd, (struct sockaddr*)&multicast, multilen) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    req.imr_multiaddr.s_addr = inet_addr(IP);
    req.imr_interface.s_addr = INADDR_ANY;
    if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &req, sizeof(req)) < 0) {
        perror("setsockopt multicast");
        exit(EXIT_FAILURE);
    }

    pthread_create(&rthread, NULL, receive, NULL);
    pthread_create(&sthread, NULL, sender, NULL);

    pthread_join(sthread, NULL);
    pthread_join(rthread, NULL);

    return 0;
}

