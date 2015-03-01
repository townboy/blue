#include "../server.h"
using namespace server;
int main () {
	Server m_server;
	m_server.init_server();
	m_server.start_server();
	return 0;
}
