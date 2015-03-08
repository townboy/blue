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
#include <set>
#include "log.h"

namespace server {
class Worker {
public:
	Worker() : MAX_THREADS(10), MAX_ROOM_THREADS(5) {}

	int init_worker(std::queue<int> *t_queue, pthread_mutex_t *mutex) {
		task_queue = t_queue;
		task_queue_mutex = mutex;
		tids = NULL;
		tids = new pthread_t[MAX_THREADS];
		pthread_mutex_init(&online_player_mutex, NULL);

		wait_queue = NULL;
		wait_queue = new std::queue<int>;
		pthread_mutex_init(&wait_queue_mutex, NULL);
		room_tids = NULL;
		room_tids = new pthread_t[MAX_ROOM_THREADS];

		LOG_DEBUG << "worker init ok !" << std::endl;
	}

	int start_worker();
	int get_online_number() {
		int x;
		pthread_mutex_lock(&online_player_mutex);
		x = online_player.size();
		pthread_mutex_unlock(&online_player_mutex);
		return x;
	}
	
	~Worker() {
		if (tids != NULL) {
			delete [] tids;
			tids = NULL;
		}

		if (room_tids != NULL) {
			delete [] room_tids;
			room_tids = NULL;
		}
		
		if (wait_queue != NULL) {
			delete wait_queue;
			wait_queue = NULL;
		}
		pthread_mutex_destroy(&wait_queue_mutex);
		pthread_mutex_destroy(&online_player_mutex);
	}


private:
	const int MAX_THREADS;
	std::queue<int> *task_queue;
	pthread_mutex_t * task_queue_mutex;
	pthread_t *tids;
	std::set<int> online_player;
	pthread_mutex_t online_player_mutex;
	static void *general_task(void *arg);

	const int MAX_ROOM_THREADS;
	std::queue<int> *wait_queue;
	pthread_mutex_t wait_queue_mutex;
	pthread_t *room_tids;
	static void *start_game_task(void *arg);

};
}
