#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h>  

#define PORT 8888

int main() {
    char ip_addr[] = "127.0.0.5";
    char data[] = "hello server";
    int data_len = sizeof(data);
    int client_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
	int flag = 1;
	
    if (client_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
	
	if (setsockopt(client_socket, IPPROTO_IP, IP_HDRINCL, &flag, sizeof(flag))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    if ((inet_pton(AF_INET, ip_addr, &server_addr.sin_addr)) < 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }

    char packet[sizeof(struct iphdr) + sizeof(struct udphdr) + data_len];
	struct iphdr *ip_header = (struct iphdr*)packet;

    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = htons(sizeof(packet));
    ip_header->id = htons(54321);
    ip_header->frag_off = 0;
    ip_header->ttl = 64;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->saddr = inet_addr("127.0.0.1");
    ip_header->daddr = inet_addr(ip_addr);

    struct udphdr *udp_header = (struct udphdr*)(packet+sizeof(struct iphdr));

    udp_header->source = htons(8888);
    udp_header->dest = htons(7777);
    udp_header->len = htons(sizeof(struct udphdr) + data_len);
    udp_header->check = 0;

    char *data_point = packet + sizeof(struct udphdr) + sizeof(struct iphdr);
    strcpy(data_point, data);
    if (sendto(client_socket, packet, sizeof(packet), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    char buffer[80]; 
    while (1){
		int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&server_addr, &addr_len);
		if (bytes_received < 0) {
        	perror("recvfrom");
	        exit(EXIT_FAILURE);
	    }

		char *received_header = buffer;
		struct udphdr *received_udp_header = ((struct udphdr*)(received_header + 20));

		uint16_t source_port = htons(received_udp_header->source);
        uint16_t dest_port = htons(received_udp_header->dest);
		int dest_port_int = (int)dest_port;
		char *payload = buffer + 28;
		if ( dest_port_int == PORT) {
	    	printf("Пакет получен от: %s\n", inet_ntoa(server_addr.sin_addr));
			printf("Payload: %s\n", payload);

		} 

	}
    close(client_socket);
    return 0;
}
