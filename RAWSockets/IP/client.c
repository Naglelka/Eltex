#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define SIZE 255
#define MY_PORT 7000
#define PORT 7777
#define ADDRESS "127.0.0.1"

int main()
{
    struct sockaddr_in server;
    int fd = 0;
    int flag = 1;
    int res = 0;

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(ADDRESS);

    fd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    res = setsockopt(fd, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag));
    if (res < 0) {
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    while(1) {
        socklen_t len = sizeof(struct sockaddr_in);
        int recvByte = 0;
        int sendByte = 0;
        int length;

        printf("Enter your message: ");
        char *message = malloc((SIZE - len) * sizeof(char));
        memset(message, 0, SIZE - len);
	    fgets(message, SIZE - len, stdin);
        char *packet = malloc(SIZE * sizeof(char));
	    memmove(packet + sizeof(struct iphdr) + sizeof(struct udphdr), message, strlen(message) - 1);

        length = strlen(message) - 1 + sizeof(struct udphdr);

        struct iphdr *ipHeader;
        ipHeader = (struct iphdr *)packet;
        ipHeader->ihl = 5;
        ipHeader->version = 4;
        ipHeader->tos = 0;
        ipHeader->id = 0;
        ipHeader->frag_off = 0;
        ipHeader->ttl = 255;
        ipHeader->protocol = IPPROTO_UDP;
        ipHeader->check = 0;
        ipHeader->saddr = inet_addr("127.0.0.1");
        ipHeader->daddr = server.sin_addr.s_addr;
        ipHeader->tot_len = 0;

        struct udphdr *udpHeader;
        udpHeader = (struct udphdr *)(packet + sizeof(struct iphdr));
        udpHeader->source = htons(MY_PORT);
        udpHeader->dest = htons(PORT);
	    udpHeader->check = 0;
	    udpHeader->len = htons(length);

        sendByte = sendto(fd, packet, length, 0, (struct sockaddr *)&server, len);
        if(sendByte == -1) {
            perror("Send error");
            exit(EXIT_FAILURE);
        }

        while(1) {
            memset(packet, 0, SIZE);
            recvByte = recvfrom(fd, packet, SIZE, 0, (struct sockaddr *)&server, &len);
            if(recvByte == -1) {
                perror("Recv error");
                exit(EXIT_FAILURE);
            }

		    udpHeader = (struct udphdr *) (packet + sizeof(struct iphdr));

            if (ntohs(udpHeader->dest) == MY_PORT) {
                printf("Received message: %s; Number of bytes: %d\n", packet + sizeof(struct iphdr) + sizeof(struct udphdr), recvByte);
                break;
		    }
        }
        free(message);
        free(packet);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}