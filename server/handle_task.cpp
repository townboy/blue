#include "handle_task.h"
#include <sstream>
#include <fcntl.h>

namespace server {
int HandleTask::handle(int t_connfd, std::set<int> *online_player, pthread_mutex_t * mutex) {
	connfd = t_connfd;	
	
	int len;
	pthread_mutex_lock(mutex);
	online_player->insert(connfd);
	pthread_mutex_unlock(mutex);

	while(1) {
		read(connfd,&len,4);
		/*if(n == 0) {
			pthread_mutex_lock(mutex);
			online_player->erase(connfd);
			pthread_mutex_unlock(mutex);
			close(connfd);
			LOG_DEBUG << "client exit !" << std::endl;
			return 0;
		}
		//LOG_DEBUG << n << std::endl;*/
		read(connfd,buff,len);
		buff[len] = '\0';

		if (!reader.parse(buff, value)) {
			LOG_ERROR << "parse json error !" << std::endl;
			return -1;
		}

		std::string m_type = value["type"].asString();
		LOG_DEBUG << m_type << std::endl;

		if (m_type == "login") {
			handle_login();
		}
		
		
	}
	return 0;
}

int HandleTask::handle_login() {
	std::string user_name = value["userName"].asString();

	MYSQL *mysql_conn;
	MYSQL_RES *res;
	MYSQL_ROW row;
	mysql_conn = mysql_init(NULL);
	if (!mysql_real_connect(mysql_conn, "localhost", "root", "123456", "game_database", 0, NULL, 0)) {
		LOG_ERROR << "%s" <<  mysql_error(mysql_conn) << std::endl;
		return -1;
	}

	std::string query_string  = "select * from user where userName = " + user_name;
	if (mysql_query(mysql_conn, query_string.c_str())) {	
		LOG_ERROR << "%s" <<  mysql_error(mysql_conn) << std::endl;
		return -1;
	}

	res = mysql_use_result(mysql_conn);
	
	if (res->row_count != 0) {
		row = mysql_fetch_row(res);
		printf("%s\n",row[0]);
	} else {
		
	}

	mysql_free_result(res);
	mysql_close(mysql_conn);
	printf("finish!\n");
}
/*
int HandleTask::handle(int t_connfd) {
	
	connfd = t_connfd;
	read(connfd, buff, 5000);
	//printf("handle task %d %s\n", strlen(buff), buff);
	if (!reader.parse(buff, value)) {
		LOG_ERROR << "parse json error !" << std::endl;
		return -1;
	}
	std::string m_type = value["type"].asString();
	//LOG_DEBUG << m_type << std::endl;
	
	if (m_type == "get_upload_id") {
		handle_get_upload_id();
	} else if (m_type == "upload_file"){
		std::cout << buff << std::endl;
		handle_upload_file();
	}

	return 0;
}

int HandleTask::handle_get_upload_id() {
	Json::Value content;
	content["status"] = "ok";
	Json::Value data;
		
	long long tmp_id = (long long)time(NULL);
	std::stringstream ss;
	ss << tmp_id;
		
	std::string upload_id ;
	ss >> upload_id;

	data["upload_id"] = Json::Value(upload_id);
	content["data"] = data;
	std::string tmp = content.toStyledString();
	strncpy(buff, tmp.c_str(), 5000);
	if (write(connfd, buff, strlen(buff)) == -1) {
		LOG_DEBUG <<  "write error !" << std::endl;
		return -1;
	}

	return 0;
}

int HandleTask::handle_upload_file() {

	std::string m_data = value["data"].asString();
	std::string m_file = value["file_name"].asString();
	m_file = m_file;
	int fp = open(m_file.c_str(), O_WRONLY | O_CREAT);
	if (fp < 0) {
		LOG_ERROR << "open file error" << std::endl;
		return -1;
	}
	while(1) {
		std::string m_data = value["data"].asString();
		std::string m_file = value["file_name"].asString();
		m_file = "data//" + m_file;
		
		LOG_DEBUG << "data length " << m_data.length() << std::endl;
		write(fp, m_data.c_str(), m_data.length());
	
		Json::Value content;
		content["status"] = "ok";
		std::string tmp = content.toStyledString();
		strncpy(buff, tmp.c_str(), 5000);
		if (write(connfd, buff, strlen(buff)) == -1) {
			LOG_DEBUG <<  "write error !" << std::endl;
			return -1;
		}


		int n = read(connfd, buff, 5000);
		if (n == -1) {
			LOG_DEBUG << "read error !" << std::endl;
			return -1;
		} else if(n == 0) {
			close(fp);
			break;
		} else {
			if (!reader.parse(buff, value)) {
				LOG_ERROR << "parse json error !" << std::endl;
				return -1;
			}
		}
	}
	return 0;
}

*/
}
