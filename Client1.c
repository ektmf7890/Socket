#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define BUF_LEN 256
#define PORT 8080
#define IP "127.0.0.1"


int main() {
	char buf[BUF_LEN];
	struct sockaddr_in serv_addr;

	int my_fd;
	int len, msg_size;

	if((my_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("my socket error\n");
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_port = htons(PORT);

	if(connect(my_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		printf("connect error\n");
		return -1;
	}

	while(1) {
		scanf("%s", buf);
		msg_size = strlen(buf) + 1;

		if(write(my_fd, buf, msg_size) < 0) {
			printf("write error\n");
			return -1;
		}
		else{
			memset(buf, 0, sizeof(buf));
			read(my_fd, buf, BUF_LEN);
			printf("%s\n", buf);
		}
		close(my_fd);
		return 0;
	}
}
