#include <stdio.h>
#include <string>
#include "jsoncpp/json.h"

struct FootballerData {
	int strength;
	int speed;
};

struct UserData {
	std::string user_name;
	int potency;
	FootballerData player[4];
};

struct FrameSyncData {
	int frame_num;
	int m_vx[4], m_vy[4];
	int m_dir[4];

	int o_vx[4], o_vy[4];
	int o_dir[4];

	std::string status;
};

struct FrameUpdaData {
	int m_vx[4], m_vy[4], m_dir[4];
	std::string status;
};
