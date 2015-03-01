#include "handle_task.h"
#include <sstream>
#include <fcntl.h>

namespace server {
int HandleTask::handle(int t_connfd, std::set<int> *online_player, pthread_mutex_t * mutex) {
	connfd = t_connfd;	
	int len;
	std::stringstream  ss;
	
	pthread_mutex_lock(mutex);
	int online_num = online_player->size() + 1;
	ss << online_num;
	std::string ts;
	ss >> ts;
	strcpy(buff, ts.c_str());
	len = strlen(buff);
	write(connfd, &len, 4);
	write(connfd, buff, len);
	online_player->insert(connfd);
	pthread_mutex_unlock(mutex);

	while(1) {
		int n = read(connfd,&len,4);
		if(n == 0) {
			pthread_mutex_lock(mutex);
			online_player->erase(connfd);
			pthread_mutex_unlock(mutex);
			close(connfd);
			LOG_DEBUG << "client exit !" << std::endl;
			return 0;
		}
		//LOG_DEBUG << n << std::endl;
		read(connfd,buff,len);
		buff[len] = '\0';
		
		std::set<int>::iterator it;
		for (it = online_player->begin(); it!=online_player->end(); it++) {
			int fd = *it;
			if(fd == connfd) 
				continue;
			write(fd, &len, 4);
			write(fd, buff, len);
		}
		/*
		if((*fd_count) >= 2) {
			if (connfd == 4) {
				write(5, &len, 4);
				write(5, buff, len);
			} else {
				write(4, &len, 4);
				write(4, buff, len);
			}
		}*/
/*
		
		LOG_DEBUG << len << " " << buff << std::endl;
		//strcpy(buff, "huangshuai");
		//len = strlen(buff);
		write(connfd, &len, 4);
		write(connfd, buff, len);*/
	}
	return 0;
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
