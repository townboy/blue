#include "../worker.h"
using namespace server;
int main() {
	
	std::queue<int> task;
	Worker m_worker;
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	
	pthread_mutex_lock(&mutex);
	task.push(10);
	task.push(15);
	task.push(12);
	task.push(1);
	pthread_mutex_unlock(&mutex);

	m_worker.init_worker(&task, &mutex);
	m_worker.start_worker();

	return 0;
}
