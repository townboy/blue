#include <stdio.h>
#include <queue>
#include <pthread.h>
#include "../master.h"
using namespace server;
int main() {
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);

	Master m_master;

	std::queue<int> q;
	m_master.init_master(&q, &mutex);
	m_master.start_master();
	return 0;
}
