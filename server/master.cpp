#include "master.h"

namespace server{

int Master::start_master() {
	if (listen(listenfd, 10) == -1) {
		printf("error: %d %s\n", errno, strerror(errno));
		return -1;
	}
	
	int connfd;
	
	while (1) {
		connfd = accept(listenfd, (sockaddr *)NULL, NULL);
		LOG_DEBUG << connfd << std::endl;

		if (connfd == -1) {
			printf("error: %d %s\n", errno, strerror(errno));
			return -1;
		}

		pthread_mutex_lock(task_queue_mutex);
		task_queue->push(connfd);
		pthread_mutex_unlock(task_queue_mutex);
	}
	
	
}

}
