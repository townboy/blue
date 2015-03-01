#include "server.h"
namespace server {
void *Server::start_master(void *arg) {
	Server *tmp = (Server *)arg;
	tmp->m_master->start_master();
	pthread_exit((void *)0);
}

void *Server::start_worker(void *arg) {
	Server *tmp = (Server *)arg;
	tmp->m_worker->start_worker();
	pthread_exit((void *)0);
}

}
