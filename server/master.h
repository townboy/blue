#include <iostream>
#include <queue>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "log.h"

namespace server{
class Master {

public:
	int init_master(std::queue<int> *t_queue, pthread_mutex_t *mutex) {
		if (t_queue == NULL) {
			return -1;
			LOG_ERROR << "init task_queue error!" << std::endl; 
		}

		task_queue = t_queue;
		task_queue_mutex = mutex;

		listenfd = socket(AF_INET, SOCK_STREAM, 0);
		if(listenfd == -1) {
			printf("error: %d %s\n", errno, strerror(errno));
			return -1;
		}
		
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = htonl(INADDR_ANY);
		server_address.sin_port = htons(8897);
	
		if (bind(listenfd, (sockaddr *)&server_address, sizeof(server_address)) == -1) {
			printf("error: %d %s\n", errno, strerror(errno));
			return -1;
		}
		LOG_DEBUG << "master init ok !" <<std::endl;
	}

	int start_master();
	
	~Master() {
		close(listenfd);
	}
	

private:
	int listenfd;
	sockaddr_in server_address;
	std::queue<int> *task_queue;
	
	pthread_mutex_t * task_queue_mutex;

};


}
