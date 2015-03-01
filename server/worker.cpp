#include "worker.h"
#include "handle_task.h"

namespace server{

void * Worker::thread_function(void *arg) {
	Worker *tmp = (Worker*)arg;
	int connfd = -1;
	while(1) {
		pthread_mutex_lock(tmp->task_queue_mutex);
		if (!tmp->task_queue->empty()) {
			connfd = tmp->task_queue->front();
			tmp->task_queue->pop();
		}
		pthread_mutex_unlock(tmp->task_queue_mutex);
		if(connfd != -1) {
			LOG_DEBUG << connfd << std::endl;
			HandleTask m_handle;
			m_handle.handle(connfd, &tmp->online_player, &tmp->online_player_mutex);
			close(connfd);
		}

		connfd = -1;
	}
	
	pthread_exit((void *)0);
}

int Worker::start_worker() {
	int connfd;
	for (int i = 0; i < MAX_THREADS; i++) {
		pthread_create(&tids[i], NULL, thread_function, (void *)this);
	}

	for (int i = 0; i < MAX_THREADS; i++) {
		pthread_join(tids[i], NULL);
	}


}

}
