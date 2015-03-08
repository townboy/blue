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
#include "master.h"
#include "worker.h"

namespace server {
class Server {
public:
	int init_server() {
		m_master = NULL;
		m_worker = NULL;
		task_queue = NULL;

		task_queue = new std::queue<int>();
		m_master = new Master();
		m_worker = new Worker();
		
		if (task_queue == NULL || m_master == NULL || m_worker == NULL) {
			LOG_ERROR << "init server error" <<std::endl;
			return -1;
		}
		pthread_mutex_init(&task_queue_mutex, NULL);
		LOG_DEBUG << "server init ok !" << std::endl;
	}

	int start_server() {
		m_master->init_master(task_queue, &task_queue_mutex);
		m_worker->init_worker(task_queue, &task_queue_mutex);
		
		pthread_create(&master_tid, NULL, start_master, (void *)this);
		pthread_create(&worker_tid, NULL, start_worker, (void *)this);

		//pthread_join(master_tid, NULL);
		//pthread_join(worker_tid, NULL);
		pthread_detach(master_tid);
		pthread_detach(worker_tid);
		while(1) {
			char command[10];
			scanf("%s", command);
			if(command[0] == 'q') {
				pthread_cancel(master_tid);
				pthread_cancel(worker_tid);
				break;
			} else if(command[0] == 's') {
				int num = m_worker->get_online_number();
				printf("Online number: %d!\n", num);
			}
		}
		return 0;
	}

	~Server() {
		if (m_master != NULL)
			delete m_master;
		if (m_worker != NULL) 
			delete m_worker;
		if(task_queue != NULL)
			delete task_queue;
		pthread_mutex_destroy(&task_queue_mutex);
	}

private:
	std::queue<int> *task_queue;
	pthread_mutex_t task_queue_mutex;
	Master *m_master;
	Worker *m_worker;
	pthread_t master_tid;
	pthread_t worker_tid;

	static void *start_master(void *arg);
	static void *start_worker(void *arg);

};
}
