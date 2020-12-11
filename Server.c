// 209073027 윤다영

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_LEN 256

int main(){
	char buf[BUF_LEN];
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	socklen_t len;
	int serv_fd;
	int cli_fd;
	int ret = 0;

	pid_t childpid;

	// socket():소켓 생성, 인터넷 프로토콜을 사용하며 SOCK_STREAM을 통해 TCP를 사용함을 명시.
	if((serv_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("server socket error!\n");
		return -1;
	}
	
	memset(&server_addr, 0, sizeof(server_addr));

	// 원격 소켓의 인터넷 주소 저장 sin_addr에는 IP주소가, sin_port에는 포트 번호가 저장.
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

		// accpet(): 수신 대기열에서 클라이언트의 연결을 기다리고 클라이언트의 주소가 client_addr에 저장됨.
		cli_fd = accept(serv_fd, (struct sockaddr*)&client_addr, &len);
		if(cli_fd < 0){
			printf("client accept fail\n");
			return -1;
		}

		else{
			printf("Client connected from: %s:%d\n", inet_ntoa(client_addr.sin_addr), (int)ntohs(client_addr.sin_port));

		}
			
		// 각 클라인언트마다 자식프로세스를 생성->멀티프로세스 기반 서버/클라이언트
		childpid= fork();
		if(childpid == 0) {
			if(close(serv_fd) < 0){
        	                printf("close socket error\n");
                	        return -1;
                        }
			
                        while(1){
				char buffer[32];
				// recv함수는 상대 소켓이 연결을 닫았을 경우 0을 반환한다
				if(recv(cli_fd, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0){
					close(cli_fd);
					printf("Client disconnected from: %s:%d\n", inet_ntoa(client_addr.sin_addr), (int)ntohs(client_addr.sin_port));
					return 0;
				}
				// 클라이언트 함수로부터 echo메세지를 읽어드림.
                                if(ret = read(cli_fd, buf, 256) < 0){
                                        printf("read error\n");
                                        return -1;
                                }
                                else {
					// echo 메세지를 다시 클라이언트에게 전송.
                                        if(write(cli_fd, buf, 256) < 0) {
                                                printf("write error\n");
                                                return -1;
                                        }
                        	}
			}
		}

		else if (childpid == -1){
			printf("fork error\n");
			return -1;
		}
	}

	return 0;
}

