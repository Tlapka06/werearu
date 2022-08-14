#include "werearu.h"

char buffer[BUFFER_LENGTH];
char hostname[BUFFER_LENGTH];

char magic[] = MAGIC;
char magicResponse[] = MAGIC_RESPONSE;
socklen_t clientAddrLen = sizeof(struct sockaddr_in);

int main(int argc, char *argv[]) {
	struct sockaddr_in loopbackAddr = {.sin_family = AF_INET, .sin_port = htons(WHEREAREU_PORT), .sin_addr.s_addr = htonl(INADDR_ANY)};
	struct sockaddr_in client_addr;
	int msg_len;
	
	// buffer init with magic
	memcpy(hostname, magic, SIZEOF_MAGIC);
	if (gethostname(hostname+SIZEOF_MAGIC, MAX_NAME_LENGTH) == -1) {
		perror("Failed to get hostname");
		exit(1);
	}
	printf("Running werearud for %s\n", hostname+SIZEOF_MAGIC);
	
	int sock = socket(PF_INET,SOCK_DGRAM,0);
	
	if (sock == -1) {
		perror("Failed to create a socket");
		exit(1);
	}
	
	int value_true = 1;
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const void *)&value_true, sizeof(int))) {
		perror("Failed to setup the socket");
		exit(1);
	}
	
	if (bind(sock, (struct sockaddr *)&loopbackAddr, sizeof(struct sockaddr_in)) == -1) {
		perror("Failed to bind the loopback address");
		exit(1);
	}
	
	
	while ((msg_len = recvfrom(sock, buffer, BUFFER_LENGTH, 0, (struct sockaddr *)&client_addr, &clientAddrLen)) != -1) {
		// end buffer with 0
		buffer[msg_len] = '\0';
		
		// Is it for me?
		if (strncmp(buffer, hostname, BUFFER_LENGTH) == 0) {
			printf("%s asked where i am\n", inet_ntoa(client_addr.sin_addr));
			
			memcpy(buffer, magicResponse, SIZEOF_MAGIC);
			if (sendto(sock, buffer, msg_len, 0, (struct sockaddr *)&client_addr, clientAddrLen) != msg_len) {
				perror("Failed to send answer");
			}
		}
	}
}