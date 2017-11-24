#include "SceneMgr.h"

SceneMgr::SceneMgr(LPVOID sock) {
	server_sock = (SOCKET)sock;
	randerer = new Randerer();
	map = new Map(1000, 1000);
}

SceneMgr::~SceneMgr() {
	delete randerer;
	delete map;
}

void SceneMgr::draw() {
	if (map != NULL) {
		randerer->drawMap(
			map->getX(),
			map->getY()
		);
	}
	if (game_State == LOGIN) {
		if (player_State == WAIT)
			randerer->printtext(220, 250, "WAIT");
		if (player_State == READY)
			randerer->printtext(220, 250, "READY");
	}
}

void SceneMgr::update(int frame_time) {
	int retval; 
	int state;
	retval = send(server_sock, (char*)&game_State, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	switch (game_State) {
	case LOGIN:
		state = player_State;
		retval = send(server_sock, (char*)&state, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		retval = recv(server_sock, (char*)&state, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		break;
	case RUNNING:
		break;
	case END:
		break;
	}
}

void SceneMgr::keyboardFunc(int key, int state) {
	if (key == KEYBOARD_R) {
		if (player_State == WAIT)
			player_State = READY;
		else if (player_State == READY)
			player_State = WAIT;
	}
}