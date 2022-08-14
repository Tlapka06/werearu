#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define WHEREAREU_PORT 1212

#define MAX_NAME_LENGTH 80
#define MAGIC {'W', 'R', 'U'}
#define MAGIC_RESPONSE {'I', 'M', 'H'}
#define SIZEOF_MAGIC 3
#define BUFFER_LENGTH MAX_NAME_LENGTH+SIZEOF_MAGIC+1