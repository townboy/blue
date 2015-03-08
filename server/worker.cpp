#include "worker.h"
#include "handle_task.h"
#include "handle_frame_sync.h"
namespace server{

void * Worker::general_task(void *arg) {
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
			m_handle.handle(connfd, &tmp->online_player, &tmp->online_player_mutex, tmp->wait_queue, &tmp->wait_queue_mutex);
		}

		connfd = -1;
	}
	
	pthread_exit((void *)0);
}

void * Worker::start_game_task(void *arg) {
	Worker *tmp = (Worker*)arg;
	int connfd1 = -1,connfd2 = -1;
	while(1) {
		pthread_mutex_lock(&tmp->wait_queue_mutex);
		if (tmp->wait_queue->size() >= 2) {
			connfd1 = tmp->wait_queue->front();
			tmp->wait_queue->pop();
			
			connfd2 = tmp->wait_queue->front();
			tmp->wait_queue->pop();
		}
		pthread_mutex_unlock(&tmp->wait_queue_mutex);

		if (connfd1 != -1 && connfd2 != -1) {
			LOG_DEBUG << "first connfd is " << connfd1 << ". second connfd is " << connfd2 << std::endl;
			HandleFrameSync m_handle;
			m_handle.handle(connfd1, connfd2, tmp->task_queue, tmp->task_queue_mutex);
			
		}

		connfd1 = connfd2 = -1;
	}
	pthread_exit((void *)0);
}

int Worker::start_worker() {
	int connfd;
	for (int i = 0; i < MAX_THREADS; i++) {
		pthread_create(&tids[i], NULL, general_task, (void *)this);
	}

	for(int i = 0; i < MAX_ROOM_THREADS; i++) {
		pthread_create(&room_tids[i], NULL, start_game_task, (void *)this);
	}

	for (int i = 0; i < MAX_THREADS; i++) {
		if (i < MAX_ROOM_THREADS) {
			pthread_join(room_tids[i], NULL);
		}
		pthread_join(tids[i], NULL);
	}


}

}
