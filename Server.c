#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8000
#define BUF_LEN 256

int main(){
	char buf[BUF_LEN];
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int ret=0, count;
	socklen_t len;
	int serv_fd;
	int cli_fd;

	pid_t childpid;

	// socket():소켓 생성 (domain, type, protocol)
	if((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("server socket error!\n");
		return -1;
	}
	

	memset(&server_addr, 0, sizeof(server_addr));
	// 원격 소켓의 인터넷 주소 저장.
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(PORT);
	
	// bind(): 소켓을 서버의 인터넷 주소에 묶어줌.
	if(bind(serv_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		printf("server bind error\n");
		return -1;
	}

	// listen(): 수신대기열을 만듦. 서버가 한번에 처리할 수 있는 클라이언트 개수를 지정.
	if(listen(serv_fd, 10) < 0) {
		printf("server listen error\n");
		return -1;
	}


	while(1){
		len = sizeof(client_addr);

		// accpet(): 수신 대기열에서 연결을 기다림. 
		cli_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &len);
		if(cli_fd < 0){
			printf("client accept fail\n");
			return -1;
		}


		if((childpid = fork()) == 0){
			if(close(serv_fd) < 0){
				printf("close socket error\n");
				return -1;
			}

			while(1){
				if(ret = read(cli_fd, buf, 256) < 0){
					printf("read error\n");
					return -1;
				}
				else {
					if(write(cli_fd, buf, 256) < 0) {
						printf("write error\n");
						return -1;
					}
				}
			}
		}

		else if(childpid == 1) {
			printf("fork error\n");
			return -1;
		}
		close(cli_fd);
	}
}

