#include "handle_task.h"
#include <sstream>
#include <fcntl.h>
#include <cstdlib>

namespace server {
int HandleTask::handle(int t_connfd, std::set<int> *online_player, pthread_mutex_t * mutex) {
	connfd = t_connfd;	
	
	int len;
	pthread_mutex_lock(mutex);
	online_player->insert(connfd);
	pthread_mutex_unlock(mutex);

	while(1) {
		int n = read(connfd, &len, 4);
		if(n == 0) {
			pthread_mutex_lock(mutex);
			online_player->erase(connfd);
			pthread_mutex_unlock(mutex);
			close(connfd);
			LOG_DEBUG << "client exit !" << std::endl;
			return 0;
		}
		read(connfd,buff,len);
		buff[len] = '\0';
		LOG_DEBUG << buff << std::endl;
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
		LOG_ERROR <<  mysql_error(mysql_conn) << std::endl;
		return -1;
	}

	std::string query_string  = "select * from user where userName = \"" + user_name + "\";";
	LOG_DEBUG << query_string << std::endl;
	if (mysql_query(mysql_conn, query_string.c_str())) {	
		LOG_ERROR <<  mysql_error(mysql_conn) << std::endl;
		return -1;
	}

	res = mysql_use_result(mysql_conn);

	Json::Value response;
	response["code"] = 200;
	response["status"] = "OK";
	Json::Value content;
	if((row = mysql_fetch_row(res)) != NULL) {
		response["message"] = "a exist user!";
		
		content["userName"] = user_name;
		content["potency"] = row[1];

		std::string play_id[4];
		play_id[0] = row[2];
		play_id[1] = row[3];
		play_id[2] = row[4];
		play_id[3] = row[5];
		mysql_free_result(res);

		for (int i = 0; i < 4; i++) {
			query_string = "select * from footballer where playId = " + play_id[i] + ";";
			LOG_DEBUG << query_string << std::endl;
			if (mysql_query(mysql_conn, query_string.c_str())) {	
				LOG_ERROR <<  mysql_error(mysql_conn) << std::endl;
				return -1;
			}

			MYSQL_RES *footballer_res = mysql_use_result(mysql_conn);
			MYSQL_ROW footballer_row = mysql_fetch_row(footballer_res);
			std::string strength = footballer_row[1];
			std::string speed = footballer_row[2];
			mysql_free_result(footballer_res);

			Json::Value player;
			player["strength"] = atoi(strength.c_str());
			player["speed"] = atoi(speed.c_str());

			std::string player_name;
			if(i ==0) {
				player_name = "player1";
			} else if (i == 1) {
				player_name = "player2";
			} else if (i == 2) {
				player_name = "player3";
			} else {
				player_name = "player4";
			}
			content[player_name] = player;
		}
		
	} else {
		response["message"] = "create a new user!";
		std::string play_id[4];
		for(int i = 0; i < 4; i++) {
			query_string = "insert into footballer(strength, speed) values(10, 10)";
			if (mysql_query(mysql_conn, query_string.c_str())) {	
				LOG_ERROR <<  mysql_error(mysql_conn) << std::endl;
				return -1;
			}

			query_string = "select @@identity;";
			if (mysql_query(mysql_conn, query_string.c_str())) {	
				LOG_ERROR <<  mysql_error(mysql_conn) << std::endl;
				return -1;
			}

			MYSQL_RES *footballer_res = mysql_use_result(mysql_conn);
			MYSQL_ROW footballer_row = mysql_fetch_row(footballer_res);
			play_id[i]= footballer_row[0];
			mysql_free_result(footballer_res);

		}
		std::ostringstream ss;
		ss << "insert into user values(\"" << user_name << "\",10," << play_id[0] << "," << play_id[1] << "," << play_id[2] << "," << play_id[3] << ")";
		query_string = ss.str();
		LOG_DEBUG << query_string << std::endl;
		if (mysql_query(mysql_conn, query_string.c_str())) {	
			LOG_ERROR << "%s" <<  mysql_error(mysql_conn) << std::endl;
			return -1;
		}
		content["userName"] = user_name;
		content["potency"] = 10;
		Json::Value player;
		player["strength"] = 10;
		player["speed"] = 10;
		content["player1"] = player;
		content["player2"] = player;
		content["player3"] = player;
		content["player4"] = player;
	}
	response["content"] = content;
	mysql_close(mysql_conn);
	
	strcpy(buff, response.toStyledString().c_str());
	int len = strlen(buff);
	
	if(write(connfd, &len, 4) == -1) {
		LOG_ERROR << "write len error!" << std::endl;
		exit(-1);
	}

	if(write(connfd, buff, strlen(buff)) == -1) {
		LOG_ERROR << "write buff error!" << std::endl;
		exit(-1);
	}

	LOG_DEBUG << buff << std::endl;
	LOG_DEBUG << "finish!" << std::endl;
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
