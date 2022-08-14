// werearu
// Copyright 2022 Jakub Janík

#include "werearu.h"
#include <time.h>
#define TIMEOUT 3000

char buffer[BUFFER_LENGTH];
char hostnameBuffer[BUFFER_LENGTH];

char magic[] = MAGIC;
char magicResponse[] = MAGIC_RESPONSE;
int serverAddrLen = sizeof(struct sockaddr_in);
struct timeval valueTimeout = {.tv_sec = TIMEOUT/1000, .tv_usec = TIMEOUT%1000};

int main(int argc, char *argv[]) {
	struct sockaddr_in broadcast_addr = {.sin_family = AF_INET, .sin_port = htons(WHEREAREU_PORT), .sin_addr.s_addr = htonl(INADDR_BROADCAST)};
	struct sockaddr_in server_addr;
	int msg_len;
	int elapsed_time = 0;
	int value_true = 1;
	clock_t clock_before;
	
	// help
	if (argc != 2) {
		fprintf(stderr, "werearu - program for address 
discovery\nusage: werearu hostname\n");
		exit(0);
	}
	
	// buffer init with magic
	memcpy(hostnameBuffer, magic, SIZEOF_MAGIC);
	// get server hostname from arg
	strncpy(hostnameBuffer+SIZEOF_MAGIC, argv[1], MAX_NAME_LENGTH);
	
	
	int sock = socket(PF_INET,SOCK_DGRAM,0);
	if (sock == -1) {
		perror("Failed to create a socket");
		exit(1);
	}
	
	// enable broadcast and set recv timeout
	if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (const void *)&value_true, sizeof(value_true)) == -1 ||\
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const void *)&valueTimeout, sizeof(struct timeval)) == -1) {
		perror("Failed to setup the socket");
		exit(1);
	}
	
	// send question
	if (sendto(sock, hostnameBuffer, BUFFER_LENGTH, 0, (struct sockaddr *)&broadcast_addr, sizeof(struct sockaddr_in)) == -1) {
		perror("Failed to broadcast a message");
		exit(1);
	}
	
	// set MAGIC in hostnameBuffer to MAGIC_RESPONSE
	memcpy(hostnameBuffer, magicResponse, SIZEOF_MAGIC);
	
	// timeout loop
	clock_before = clock();
	do {
		// recv answer
		if (recvfrom(sock, buffer, BUFFER_LENGTH, 0, (struct sockaddr *)&server_addr, (socklen_t *)&serverAddrLen) == -1) {
			break;
		}
		// is it my answer?
		if (strncmp(hostnameBuffer, buffer, BUFFER_LENGTH) == 0) {
			fprintf(stderr, "%s has ip %s\n", argv[1], inet_ntoa(server_addr.sin_addr));
			if (!isatty(STDOUT_FILENO)) {
				printf("%s\n", inet_ntoa(server_addr.sin_addr));
			}
			exit(0);
		}
		
		clock_t difference = clock() - clock_before;
		elapsed_time = difference * 1000 / CLOCKS_PER_SEC;
	} while ( elapsed_time < TIMEOUT );
	
	fprintf(stderr, "%s have not answered\n", argv[1]);
	exit(1);
}
