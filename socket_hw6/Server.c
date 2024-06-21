#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define PORT_SRC 7777
#define PORT_DST 8888
#define BUFFER_SIZE 2048

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
    int server_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (server_socket < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    char interface[] = "lo";
    struct ifreq ifr;
    memset(&ifr, 0, sizeof(ifr));
    strncpy(ifr.ifr_name, interface, IFNAMSIZ - 1);
    if (ioctl(server_socket, SIOCGIFINDEX, &ifr) < 0) {
        perror("ioctl");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_ll server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sll_family = AF_PACKET;
    server_addr.sll_protocol = htons(ETH_P_ALL);
    server_addr.sll_ifindex = ifr.ifr_ifindex;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    struct sockaddr_ll client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received < 0) {
            perror("recvfrom");
            close(server_socket);
            exit(EXIT_FAILURE);
        }

        struct ethhdr *eth_header = (struct ethhdr *)buffer;
        struct iphdr *ip_header = (struct iphdr *)(buffer + sizeof(struct ethhdr));
        struct udphdr *udp_header = (struct udphdr *)(buffer + sizeof(struct ethhdr) + sizeof(struct iphdr));
        
        if (ntohs(udp_header->dest) == PORT_DST) {
            char *data = buffer + sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr);
            printf("Received packet from %s:%d\n", inet_ntoa(*(struct in_addr *)&ip_header->saddr), ntohs(udp_header->source));
            printf("Data: %s\n", data);

            char reply[] = "hello client";
            memcpy(data, reply, sizeof(reply));
            udp_header->len = htons(sizeof(struct udphdr) + sizeof(reply));
            udp_header->check = 0;
            udp_header->check = checksum((uint16_t *)udp_header, sizeof(struct udphdr) + sizeof(reply));
            ip_header->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(reply));
            ip_header->check = 0;
            ip_header->check = checksum((uint16_t *)ip_header, sizeof(struct iphdr));

            if (sendto(server_socket, buffer, sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct udphdr) + sizeof(reply), 0, (struct sockaddr *)&client_addr, addr_len) < 0) {
                perror("sendto");
                close(server_socket);
                exit(EXIT_FAILURE);
            }
        }
    }

    close(server_socket);
    return 0;
}

