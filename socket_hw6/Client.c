#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ether.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <linux/if_packet.h>
#define PORT 8888

uint16_t checksum(uint16_t *buffer, int length) {
    unsigned long sum = 0;
    while (length > 1) {
        sum += *buffer++;
        length -= 2;
    }
    if (length > 0) {
        sum += *(unsigned char *)buffer;
    }
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return (uint16_t)(~sum);
}

int main() {
    char ip_addr[] = "127.0.0.5";
    char mac_addr[] = "0A-00-27-00-00-0F";
    char data[] = "hello server";
    int data_len = strlen(data);

    int client_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (client_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_ll server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sll_family = AF_PACKET;
    server_addr.sll_protocol = htons(ETH_P_ALL);
    server_addr.sll_ifindex = if_nametoindex("lo");
    server_addr.sll_halen = ETH_ALEN;

    char packet[sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + data_len];

    struct ethhdr *eth_header = (struct ethhdr *)packet;
    memset(eth_header->h_source, 0, ETH_ALEN);
    memcpy(eth_header->h_dest, server_addr.sll_addr, ETH_ALEN);
    eth_header->h_proto = htons(ETH_P_IP);

    struct iphdr *ip_header = (struct iphdr *)(packet + sizeof(struct ethhdr));
    ip_header->ihl = 5;
    ip_header->version = 4;
    ip_header->tos = 0;
    ip_header->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + data_len);
    ip_header->id = htons(54321);
    ip_header->frag_off = 0;
    ip_header->ttl = 64;
    ip_header->protocol = IPPROTO_UDP;
    ip_header->saddr = inet_addr("127.0.0.1");
    ip_header->daddr = inet_addr(ip_addr);
    ip_header->check = 0;

    struct udphdr *udp_header = (struct udphdr *)(packet + sizeof(struct ethhdr) + sizeof(struct iphdr));
    udp_header->source = htons(8888);
    udp_header->dest = htons(PORT);
    udp_header->len = htons(sizeof(struct udphdr) + data_len);
    udp_header->check = 0;

    char *data_ptr = (char *)(packet + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr));
    strcpy(data_ptr, data);

    udp_header->check = checksum((uint16_t *)udp_header, sizeof(struct udphdr) + data_len);
    ip_header->check = checksum((uint16_t *)ip_header, sizeof(struct iphdr));

    if (sendto(client_socket, packet, sizeof(packet), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    char buffer[80];
    while (1) {
        int bytes_received = recvfrom(client_socket, buffer, sizeof(buffer), 0, NULL, NULL);
        if (bytes_received < 0) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }

        struct udphdr *received_udp_header = (struct udphdr *)(buffer + sizeof(struct iphdr) + sizeof(struct ethhdr));
        uint16_t dest_port = ntohs(received_udp_header->dest);
        if (dest_port == PORT) {
            printf("Пакет получен от: %s\n", ip_addr);
            printf("Payload: %s\n", buffer + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(struct ethhdr));
            break;
        }
    }

    close(client_socket);
    return 0;
}

