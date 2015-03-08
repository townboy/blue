#include "network_tool.h"
#include <sstream>

NetworkTool * NetworkTool::s_instance = NULL;

int NetworkTool::login(std::string user_name, UserData * user_data) {
	if (socket_fd == -1) {
		printf("please connect server!\n");
		return -1;
	}
	
	if (user_data == NULL) {
		printf("user_data point is NULL");
		return -1;
	}

	std::stringstream ss;
	ss << "{\"type\":\"login\" , \"userName\":\"" << user_name << "\"}";
	strcpy(buff, ss.str().c_str());
	int len = strlen(buff);
	if(write(socket_fd, &len, 4) == -1) {
		printf("write len error!\n");
		return -1;
	}
	if(write(socket_fd, buff, strlen(buff)) == -1) {
		printf("write buff error!\n");
		return -1;
	}

	read(socket_fd, &len, 4);
	read(socket_fd, buff, len);
	if (!reader.parse(buff, value)) {
		printf("parse json error!\n");
		return -1;
	}

	user_data->user_name = value["content"]["userName"].asString();
	user_data->potency = value["content"]["potency"].asInt();
	user_data->player[0].strength = value["content"]["player1"]["strength"].asInt();
	user_data->player[0].speed = value["content"]["player1"]["speed"].asInt();

	user_data->player[1].strength = value["content"]["player2"]["strength"].asInt();
	user_data->player[1].speed = value["content"]["player2"]["speed"].asInt();

	user_data->player[2].strength = value["content"]["player3"]["strength"].asInt();
	user_data->player[2].speed = value["content"]["player3"]["speed"].asInt();

	user_data->player[3].strength = value["content"]["player4"]["strength"].asInt();
	user_data->player[3].speed = value["content"]["player4"]["speed"].asInt();

	return 0;
}

int NetworkTool::start_game_req() {
	value["type"] = "start_game";

	strcpy(buff, value.toStyledString().c_str());
	int len = strlen(buff);
	if(write(socket_fd, &len, 4) == -1) {
		printf("write len error!\n");
		return -1;
	}
	if(write(socket_fd, buff, strlen(buff)) == -1) {
		printf("write buff error!\n");
		return -1;
	}
	return 0;
}

int NetworkTool::start_game_resp() {
	int len;
	read(socket_fd, &len, 4);
	read(socket_fd, buff, len);
	if (!reader.parse(buff, value)) {
		printf("parse json error!\n");
		return -1;
	}
	
	if (value["code"] == 200 && value["status"] == "successful matches") {
		return 0;
	} else {
		return -1;
	}
}

int NetworkTool::get_frame_sync_data(FrameSyncData *sync_data) {
	int len;
	read(socket_fd, &len, 4);
	read(socket_fd, buff, len);
	if (!reader.parse(buff, value)) {
		printf("parse json error!\n");
		return -1;
	}

	sync_data->frame_num = value["frameNum"].asInt();
	sync_data->status = value["status"].asString();

	for(int i = 0; i < 4; i++) {
		std::stringstream ss;
		ss << i;

		std::string m_vx = "m_vx" + ss.str();
		std::string m_vy = "m_vy" + ss.str();
		std::string m_dir = "m_dir" + ss.str();

		std::string o_vx = "o_vx" + ss.str();
		std::string o_vy = "o_vy" + ss.str();
		std::string o_dir = "o_dir" + ss.str();

		sync_data->m_vx[i] = value["control"][m_vx].asInt();
		sync_data->m_vy[i] = value["control"][m_vy].asInt();
		sync_data->m_dir[i] = value["control"][m_dir].asInt();

		sync_data->o_vx[i] = value["control"][o_vx].asInt();
		sync_data->o_vy[i] = value["control"][o_vy].asInt();
		sync_data->o_dir[i] = value["control"][o_dir].asInt();
	}
	return 0;
}

int NetworkTool::push_frame_upda_data(FrameUpdaData * upda_data) {
	for (int i = 0; i < 4; i++) {
	
		std::stringstream ss;
		ss << i;
		std::string m_vx = "m_vx" + ss.str();
		std::string m_vy = "m_vy" + ss.str();
		std::string m_dir = "m_dir" + ss.str();

		value[m_vx] = upda_data->m_vx[i];
		value[m_vy] = upda_data->m_vy[i];
		value[m_dir] = upda_data->m_dir[i];
 
	}
	value["status"] = upda_data->status;

	strcpy(buff, value.toStyledString().c_str());
	int len = strlen(buff);
	if(write(socket_fd, &len, 4) == -1) {
		printf("write len error!\n");
		return -1;
	}
	if(write(socket_fd, buff, strlen(buff)) == -1) {
		printf("write buff error!\n");
		return -1;
	}

	return 0;
}
