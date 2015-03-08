#include <iostream>
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
class HandleTask{
public:
	HandleTask() {
		buff = NULL;
		buff = new char[5000];
	}

	~HandleTask() {
		if(buff != NULL)
			delete [] buff;
		buff = NULL;
	}

	int handle(int t_connfd, std::set<int> *online_player, pthread_mutex_t *, std::queue<int> *wait_queue, pthread_mutex_t *wait_queue_mutex);
	int handle_login();
	int handle_start_game(int connfd, std::queue<int> *wait_queue, pthread_mutex_t *wait_queue_mutex);

private:
	char *buff;
	int connfd;
	Json::Reader reader;
	Json::Value value;
};

}
