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
#include <netinet/if_ether.h>
#include <linux/if_packet.h>
#include <net/if.h>

#define SIZE 255
#define MY_PORT 7000
#define PORT 7777
#define ADDRESS_SERVER "192.168.0.7"
#define ADDRESS_CLIENT "192.168.0.9"

unsigned short csum(unsigned short *ptr,int nbytes)
{
	register long sum = 0;
	unsigned short oddbyte;
	register short answer;

	while(nbytes > 1) {
		sum += *ptr++;
		nbytes -= 2;
	}
	if(nbytes == 1) {
		oddbyte = 0;
		*((u_char*)&oddbyte) = *(u_char*)ptr;
		sum += oddbyte;
	}

	sum = (sum >> 16)+(sum & 0xffff);
	sum = sum + (sum >> 16);
	answer = (short)~sum;
	
	return(answer);
}

int main()
{

    unsigned char macSource[6] = {0x08, 0x00, 0x27, 0xdf, 0xdb, 0xd2};
    unsigned char macDest[6] = {0x08, 0x00, 0x27, 0x7a, 0x7e, 0x1a};

    struct sockaddr_ll server;
    int fd = 0;
    int flag = 1;
    int res = 0;

    server.sll_family = AF_PACKET;
    server.sll_ifindex = if_nametoindex("enp0s3");
    server.sll_halen = 6;
    memmove((void *)(server.sll_addr), (void *)macDest, 6);

    fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (fd == -1) {
        perror("Socket error");
        exit(EXIT_FAILURE);
    }

    while(1) {
        socklen_t len = sizeof(struct sockaddr_ll);
        int recvByte = 0;
        int sendByte = 0;
        int length;

        printf("Enter your message: ");
        char *message = malloc((SIZE - len) * sizeof(char));
        memset(message, 0, SIZE - len);
	    fgets(message, SIZE - len, stdin);
        char *packet = malloc(SIZE * sizeof(char));
	    memmove(packet + sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct udphdr), message, strlen(message) - 1);

        length = strlen(message) - 1 + sizeof(struct udphdr);

        struct ether_header *etherHeader;
        etherHeader = (struct ether_header *) packet;
        for (int i = 0; i < 6; i++) {
            etherHeader->ether_shost[i] = macSource[i];
            etherHeader->ether_dhost[i] = macDest[i];
        }
        etherHeader->ether_type = htons(ETHERTYPE_IP);

        struct iphdr *ipHeader;
        ipHeader = (struct iphdr *)(packet + sizeof(struct ether_header));
        ipHeader->ihl = 5;
        ipHeader->version = 4;
        ipHeader->tos = 0;
        ipHeader->id = htonl(54321);
        ipHeader->frag_off = 0;
        ipHeader->ttl = 255;
        ipHeader->protocol = IPPROTO_UDP;
        ipHeader->saddr = inet_addr(ADDRESS_SERVER);
        ipHeader->daddr = inet_addr(ADDRESS_CLIENT);
        ipHeader->tot_len = htons(ipHeader->ihl * 4 + sizeof(struct udphdr) + strlen(message) - 1);
	    ipHeader->check = 0;
	    ipHeader->check = csum((unsigned short *) ipHeader, ipHeader->ihl * 4);

        struct udphdr *udpHeader;
        udpHeader = (struct udphdr *)(packet + sizeof(struct ether_header) + sizeof(struct iphdr));
        udpHeader->source = htons(MY_PORT);
        udpHeader->dest = htons(PORT);
	    udpHeader->check = 0;
	    udpHeader->len = htons(length);

        sendByte = sendto(fd, packet, length + sizeof(struct ether_header) + sizeof(struct iphdr), 0, (struct sockaddr *)&server, len);
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

            etherHeader = (struct ether_header *) packet;
            short int correctMac = 1;
            for (int i = 0; i < 6; i++) {
                if (etherHeader->ether_dhost[i] != macSource[i]) {
                    correctMac = 0;
                    break;
                }
            }
            if (correctMac == 0) 
                continue;

            ipHeader = (struct iphdr *) (packet + sizeof(struct ether_header));
            struct in_addr addr;
            addr.s_addr = ipHeader->daddr;
            if (strcmp(ADDRESS_CLIENT, inet_ntoa(addr)) != 0)
                continue;

		    udpHeader = (struct udphdr *) (packet + sizeof(struct ether_header) +
		             ipHeader->ihl * 4);     
            if (ntohs(udpHeader->dest) != MY_PORT) {
                continue; 
            } else if (ntohs(udpHeader->dest) == MY_PORT) {
                printf("Received message: %s; Number of bytes: %d\n", packet + sizeof(struct ether_header) + ipHeader->ihl * 4 + sizeof(struct udphdr), recvByte);
                break;
		    }
        }
        free(message);
        free(packet);
    }
    close(fd);
    exit(EXIT_SUCCESS);
}
