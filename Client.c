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

	// socket 생성: SOCKE_STREAM을 통해 TCP 프로토콜 사용.
	if((my_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("my socket error\n");
		return -1;
	}

	memset(&serv_addr, 0, sizeof(serv_addr));

	// 서버에 접속하기 위해 IP주소와 포트 번호를 serv_addr 구조체에 저장.
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(IP);
	serv_addr.sin_port = htons(PORT);

	// connect 함수를 통해 생성된 서버에 접속. 
	if(connect(my_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
		printf("connect error\n");
		return -1;
	}

	while(1) {
		scanf("%s", buf);
		msg_size = strlen(buf) + 1;

		// write으로 서버에 echo 메세지 전송 
		if(write(my_fd, buf, msg_size) < 0) {
			printf("write error\n");
			return -1;
		}
		else{
			memset(buf, 0, sizeof(buf));
			// 서버에서 write한 echo 메세지를 버퍼에 읽어드리고 출력.
			read(my_fd, buf, BUF_LEN);
			printf("%s\n", buf);
		}
		// 소켓 연결 종료 확인 문구를 출력하고 close를 이용하여 disconnect
		printf("Disconnecting from server\n");
		close(my_fd);
		return 0;
	}
}
