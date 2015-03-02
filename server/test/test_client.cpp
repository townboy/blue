#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char ** argv) {
	int socket_fd;
	struct sockaddr_in server_address;
	if(argc != 2) {
		printf("usage\n");
		exit(0);
	}

	if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		printf("create socket error!\n");
		exit(0);
	}

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(8897);
	if(inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
		printf("inet_pton error!\n");
		exit(0);
	}

	if(connect(socket_fd, (sockaddr *)&server_address, sizeof(server_address)) == -1) {
		printf("connect error!\n");
		exit(0);
	}
	char buff[1000] = "{\"type\":\"login\" , \"userName\":\"huangshuai\"}";
	int len = strlen(buff);

	if(write(socket_fd, &len, 4) == -1) {
		printf("write len error!\n");
		exit(0);
	}
	if(write(socket_fd, buff, strlen(buff)) == -1) {
		printf("write buff error!\n");
		exit(0);
	}

	read(socket_fd, &len, 4);
	read(socket_fd, buff, len);
	printf("%s\n", buff);
	close(socket_fd);
	return 0;
}

