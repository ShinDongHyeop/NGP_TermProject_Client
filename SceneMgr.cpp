#include "SceneMgr.h"

SceneMgr::SceneMgr(LPVOID sock) {
	server_sock = (SOCKET)sock;
	randerer = new Randerer();
	map = new Map(1000, 1000);
	int retval;
	retval = recv(server_sock, (char*)&m_Player_Code, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
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
	if (game_State == RUNNING) {
		if (m_Player != NULL)
			randerer->drawPlayer(m_Player->getDrawX(), m_Player->getDrawY(), 1, 1, 1);

		if (o_Players.size() > 0) {
			for (int i = 0; i < MAX_PLAYER - 1; ++i) {
				randerer->drawPlayer(o_Players[i]->getDrawX(), o_Players[i]->getDrawY(), 1, 1, 1);
			}
		}
	}
}

void SceneMgr::update(int frame_time) {
	int retval; 
	int state;
	retval = send(server_sock, (char*)&frame_time, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
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
		player_State = state;
		if (player_State == START) {
			retval = recvn(server_sock, (char*)&playersBuf, PB_SIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			initPlayersData();
			game_State = RUNNING;
		}
		break;
	case RUNNING:
		setClientBuf();
		retval = send(server_sock, (char*)&cb, sizeof(ClientBuf), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		retval = recvn(server_sock, (char*)&playersBuf, PB_SIZE, 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		setPlayers();
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

void SceneMgr::initPlayersData() {
	m_Player = new Player(
		playersBuf[m_Player_Code].real_X, playersBuf[m_Player_Code].real_Y,
		playersBuf[m_Player_Code].hp
	);
	map->setXY(m_Player->getRealX(), m_Player->getRealY());
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (i == m_Player_Code)
			++i;
		o_Players.push_back(
			new Player(
				m_Player->getRealX(), m_Player->getRealY(),
				playersBuf[i].real_X, playersBuf[i].real_Y,
				playersBuf[i].hp
			));
	}
}

void SceneMgr::changeMove(int key, int state) {
	if (key == KEYBOARD_A) {
		if (state == 1) {
			if (mMoveState[0] == 0)
				mMoveState[0] = -1;
			else if (mMoveState[0] == 1)
				mMoveState[0] = 0;
		}
		else
			mMoveState[0] -= state;
	}

	if (key == KEYBOARD_D) {
		if (state == 1) {
			if (mMoveState[0] == 0)
				mMoveState[0] = 1;
			else if (mMoveState[0] == -1)
				mMoveState[0] = 0;
		}
		else
			mMoveState[0] += state;
	}

	if (key == KEYBOARD_W) {
		if (state == 1) {
			if (mMoveState[1] == 0)
				mMoveState[1] = 1;
			else if (mMoveState[1] == -1)
				mMoveState[1] = 0;
		}
		else
			mMoveState[1] += state;
	}

	if (key == KEYBOARD_S) {
		if (state == 1) {
			if (mMoveState[1] == 0)
				mMoveState[1] = -1;
			else if (mMoveState[1] == 1)
				mMoveState[1] = 0;
		}
		else
			mMoveState[1] -= state;
	}
	if (m_Player != NULL)
		m_Player->change_move(mMoveState);
}

void SceneMgr::setPlayers() {
	m_Player->update(
		playersBuf[m_Player_Code].real_X, playersBuf[m_Player_Code].real_Y,
		playersBuf[m_Player_Code].hp
	);
	map->setXY(m_Player->getRealX(), m_Player->getRealY());
	printf("%.5f\t%.5f\n", map->getX(), map->getY());
}