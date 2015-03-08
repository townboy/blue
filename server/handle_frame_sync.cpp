#include "handle_frame_sync.h"
namespace server{
int HandleFrameSync::handle(int t_connfd1, int t_connfd2, std::queue<int> *task_queue, pthread_mutex_t *task_queue_mutex) {
	
	connfd1 = t_connfd1;
	connfd2 = t_connfd2;
	Json::Value response;
	response["code"] = 200;
	response["status"] = "successful matches";

	strcpy(buff, response.toStyledString().c_str());
	int len = strlen(buff);

	if (write(connfd1, &len, 4) == -1) {
		LOG_ERROR << "write len error!" << std::endl;
		exit(-1);
	}

	if (write(connfd1, buff, strlen(buff)) == -1) {
		LOG_ERROR << "write buff error!" << std::endl;
		exit(-1);
	}

	if (write(connfd2, &len, 4) == -1) {
		LOG_ERROR << "write len error!" << std::endl;
		exit(-1);
	}

	if (write(connfd2, buff, strlen(buff)) == -1) {
		LOG_ERROR << "write buff error!" << std::endl;
		exit(-1);
	}

	int tmp = start_frame_sync();
	if(tmp == 0) {
		pthread_mutex_lock(task_queue_mutex);
		task_queue->push(connfd1);
		task_queue->push(connfd2);
		pthread_mutex_unlock(task_queue_mutex);
	}
	return 0;
}

int HandleFrameSync::start_frame_sync() {
	int cur_frame_num = 0;
	
	int a_vx[4], b_vx[4], a_vy[4], b_vy[4], a_dir[4], b_dir[4];
	for(int i = 0; i < 4; i++) {
		a_vx[i] = -1, b_vx[i] = -1;
		a_vy[i] = -1, b_vy[i] = -1;
		a_dir[i] = 0, b_dir[i] = 0;
	}
	std::string m_status = "running";
	while(1) {
		Json::Value response;	
		Json::Value control;
		int len;
		response["frameNum"] = cur_frame_num;
		response["status"] = m_status;
		for(int i = 0; i < 4; i++) {
			std::stringstream ss;
			ss << i;
			std::string m_vx = "m_vx" + ss.str();
			std::string m_vy = "m_vy" + ss.str();
			std::string m_dir = "m_dir" + ss.str();

			std::string o_vx = "o_vx" + ss.str();
			std::string o_vy = "o_vy" + ss.str();
			std::string o_dir = "o_dir" + ss.str();
			
			control[m_vx] = a_vx[i];
			control[m_vy] = a_vy[i];
			control[m_dir] = a_dir[i];

			control[o_vx] = b_vx[i];
			control[o_vy] = b_vy[i];
			control[o_dir] = b_dir[i];
		}

		response["control"] = control;
			
		strcpy(buff, response.toStyledString().c_str());
		len = strlen(buff);

		if (write(connfd1, &len, 4) == -1) {
			LOG_ERROR << "write len error!" << std::endl;
			exit(-1);
		}

		if (write(connfd1, buff, strlen(buff)) == -1) {
			LOG_ERROR << "write buff error!" << std::endl;
			exit(-1);
		}

		response["frameNum"] = cur_frame_num;
		response["status"] = m_status;
		for(int i = 0; i < 4; i++) {
			std::stringstream ss;
			ss << i;
			std::string m_vx = "m_vx" + ss.str();
			std::string m_vy = "m_vy" + ss.str();
			std::string m_dir = "m_dir" + ss.str();

			std::string o_vx = "o_vx" + ss.str();
			std::string o_vy = "o_vy" + ss.str();
			std::string o_dir = "o_dir" + ss.str();
			
			control[m_vx] = b_vx[i];
			control[m_vy] = b_vy[i];
			control[m_dir] = b_dir[i];

			control[o_vx] = a_vx[i];
			control[o_vy] = a_vy[i];
			control[o_dir] = a_dir[i];
		}
		response["control"] = control;
		strcpy(buff, response.toStyledString().c_str());
		len = strlen(buff);

		if (write(connfd2, &len, 4) == -1) {
			LOG_ERROR << "write len error!" << std::endl;
			exit(-1);
		}

		if (write(connfd2, buff, strlen(buff)) == -1) {
			LOG_ERROR << "write buff error!" << std::endl;
			exit(-1);
		}

		if (m_status == "end") {
			return 0;
		}

		cur_frame_num = (cur_frame_num + 5) % 60;
		
		read(connfd1, &len, 4);
		read(connfd1, buff, len);
		buff[len] = '\0';
		LOG_DEBUG << buff << std::endl;
		
		if (!reader.parse(buff, value)) {
			LOG_ERROR << "parse json error !" << std::endl;
			return -1;
		}

		for (int i = 0; i < 4; i++) {
			std::stringstream ss;
			ss << i;
			std::string m_vx = "m_vx" + ss.str();
			std::string m_vy = "m_vy" + ss.str();
			std::string m_dir = "m_dir" + ss.str();
			
			a_vx[i] = value[m_vx].asInt();
			a_vy[i] = value[m_vy].asInt();
			a_dir[i] = value[m_dir].asInt();
		}
		if (m_status == "running") {
			m_status = value["status"].asString();
		}
		
		read(connfd2, &len, 4);
		read(connfd2, buff, len);
		buff[len] = '\0';
		LOG_DEBUG << buff << std::endl;
		
		if (!reader.parse(buff, value)) {
			LOG_ERROR << "parse json error !" << std::endl;
			return -1;
		}
		

		for (int i = 0; i < 4; i++) {
			std::stringstream ss;
			ss << i;
			std::string m_vx = "m_vx" + ss.str();
			std::string m_vy = "m_vy" + ss.str();
			std::string m_dir = "m_dir" + ss.str();
			
			b_vx[i] = value[m_vx].asInt();
			b_vy[i] = value[m_vy].asInt();
			b_dir[i] = value[m_dir].asInt();
		}
		if (m_status == "running") {
			m_status = value["status"].asString();
		}
	}
	return 0;
}
}
