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
#include <time.h>
#include <set>
#include "log.h"
#include "jsoncpp/json.h"

namespace server {
class HandleTask{
public:
	HandleTask() {
		buff = new char[5000];
	}

	~HandleTask() {
		delete [] buff;
	}

	int handle(int t_connfd, std::set<int> *online_player, pthread_mutex_t *);
//	int handle_get_upload_id();
//	int handle_upload_file(); 

private:
	char *buff;
	int connfd;
	Json::Reader reader;
	Json::Value value;
};

}
