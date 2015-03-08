#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include <time.h>
#include <queue>
#include <set>
#include "log.h"
#include "jsoncpp/json.h"

namespace server {
class HandleFrameSync {
public:
	HandleFrameSync() {
		buff = NULL;
		buff = new char [5000];
	}

	~HandleFrameSync() {
		if (buff != NULL) 
			delete buff;
		buff = NULL;
	}

	int handle(int t_connfd1, int t_connfd2, std::queue<int> *task_queue, pthread_mutex_t *task_queue_mutex);
	int start_frame_sync();	

private:
	char *buff;
	int connfd1;
	int connfd2;
	Json::Reader reader;
	Json::Value value;
};
}
