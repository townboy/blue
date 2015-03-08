#include <stdio.h>
#include <iostream>
#include "network_tool.h"

NetworkTool *p;
int connect() {
	p = NetworkTool::get_instance();
	p->connect_server();
	return 0;
}
int test_login() {
	std::string user_name;
	std::cin >> user_name;
	UserData user_data;
	p->login(user_name, &user_data);
	std::cout << "user_name " << user_data.user_name << std::endl;
	std::cout << "potency " << user_data.potency << std::endl;
	for (int i = 0; i < 4; i++) {
		std::cout << i+1 << " speed " << user_data.player[i].speed << std::endl;
		std::cout << i+1 << " strength" << user_data.player[i].strength << std::endl;
	}
}
int test_start_game() {
	p->start_game_req();
	printf("waiting...\n");
	p->start_game_resp();
	printf("match successful!\n");
	return 0;
}
int test_get_frame_sync_data() {
	FrameSyncData sync_data;
	p->get_frame_sync_data(&sync_data);
	for (int i = 0; i < 4; i++) {
		printf("my: %d %d %d\n", sync_data.m_vx[i], sync_data.m_vy[i], sync_data.m_dir[i]);
		printf("ot: %d %d %d\n", sync_data.o_vx[i], sync_data.o_vy[i], sync_data.o_dir[i]);
	}
	printf("%s\n",sync_data.status.c_str());
	return 0;
}
int test_push_frame_upda_data() {
	FrameUpdaData upda_data;
	int tmp;
	std::cin >> tmp;
	for(int i = 0; i < 4; i++) {
		upda_data.m_vx[i] = tmp;
		upda_data.m_vy[i] = tmp;
		upda_data.m_dir[i] = tmp;
	}

	if(tmp ==-1)
		upda_data.status = "end";
	else
		upda_data.status = "running";

	p->push_frame_upda_data(&upda_data);
	return 0;
}
int main() {
	connect();
	test_login();
	test_start_game();
	test_get_frame_sync_data();
	while(1) {
		test_push_frame_upda_data();
		test_get_frame_sync_data();
	}
	return 0;
}
