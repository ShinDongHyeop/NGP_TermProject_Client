#include "SceneMgr.h"

SceneMgr::SceneMgr(LPVOID sock) {
	server_sock = (SOCKET)sock;
	randerer = new Randerer();
	map = new Map(1000, 1000);
	initBush();
	int retval;
	retval = recv(server_sock, (char*)&m_Player_Code, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		err_display("recv()");
	}
	int opt_val = TRUE;
	setsockopt(server_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(int));

	m_Player = NULL;
	start_time = respawn_time = 5.0f;
}

SceneMgr::~SceneMgr() {
	delete randerer;
	delete map;
}

void SceneMgr::initBush() {
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 8; ++j)
			bushs.push_back(
				new Bush(
					i * 100 + 200, j * 100 + 1000,
					map->getX(), map->getY()
				)
			);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			bushs.push_back(
				new Bush(
					i * 100 + 850, j * 100 + 1100,
					map->getX(), map->getY()
				)
			);

	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 2; ++j)
			bushs.push_back(
				new Bush(
					i * 100 + 1300, j * 100 + 1750,
					map->getX(), map->getY()
				)
			);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 6; ++j)
			bushs.push_back(
				new Bush(
					i * 100 + 600, j * 100 + 200,
					map->getX(), map->getY()
				)
			);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 6; ++j)
			bushs.push_back(
				new Bush(
					i * 100 + 1400, j * 100 + 200,
					map->getX(), map->getY()
				)
			);
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
		for (auto d : bushs)
		randerer->drawBush(d->getDrawX(), d->getDrawY(), d->getAlpha());
	}
	if (game_State == RUNNING) {
		if (player_State == START) {
			if (m_Player != NULL) {
				randerer->drawPlayer(
					m_Player->getDrawX(), m_Player->getDrawY(),
					1, 1, 1
				);
				randerer->drawBulletWay(
					m_Player->getDrawX(), m_Player->getDrawY(),
					m_Player->getLookX(), m_Player->getLookY()
				);
				randerer->drawHP(
					m_Player->getDrawX(), m_Player->getDrawY(),
					m_Player->getFullHP(), m_Player->getNowHP()
				);
			}

			if (o_Players.size() > 0) {
				for (int i = 0; i < MAX_PLAYER - 1; ++i) {
					randerer->drawPlayer(
						o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
						1, 1, 1
					);
					randerer->drawO_BulletWay(
						o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
						o_Players[i]->getLookX(), o_Players[i]->getLookY()
					);
					randerer->drawHP(
						o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
						o_Players[i]->getFullHP(), o_Players[i]->getNowHP()
					);
				}
			}
			int dec, sign;
			randerer->printtext(250, 250, fcvt(start_time, 0, &dec, &sign));
			for (auto d : bushs)
				randerer->drawBush(d->getDrawX(), d->getDrawY(), d->getAlpha());
		}
		if (player_State == PLAY) {
			if (m_Player != NULL) {
				randerer->drawPlayer(
					m_Player->getDrawX(), m_Player->getDrawY(),
					1, 1, 1
				);
				randerer->drawBulletWay(
					m_Player->getDrawX(), m_Player->getDrawY(),
					m_Player->getLookX(), m_Player->getLookY()
				);
				randerer->drawHP(
					m_Player->getDrawX(), m_Player->getDrawY(),
					m_Player->getFullHP(), m_Player->getNowHP()
				);
				
			}

			if (o_Players.size() > 0) {
				for (int i = 0; i < MAX_PLAYER - 1; ++i) {
					if (!o_Players[i]->hpZero()) {
						randerer->drawPlayer(
							o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
							1, 1, 1
						);
						randerer->drawO_BulletWay(
							o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
							o_Players[i]->getLookX(), o_Players[i]->getLookY()
						);
						randerer->drawHP(
							o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
							o_Players[i]->getFullHP(), o_Players[i]->getNowHP()
						);
					}
				}
			}
			for (auto b : items) {
				randerer->drawItem(b->getDrawX(), b->getDrawY());
			}

			for (auto b : bullets) {
				randerer->drawBullet(b->getDrawX(), b->getDrawY());
			}
			for (auto d : bushs)
				randerer->drawBush(d->getDrawX(), d->getDrawY(), d->getAlpha());
		}
		if (player_State == DIE) {
			if (o_Players.size() > 0) {
				for (int i = 0; i < MAX_PLAYER - 1; ++i) {
					if (!o_Players[i]->hpZero()) {
						randerer->drawPlayer(
							o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
							1, 1, 1
						);
						randerer->drawO_BulletWay(
							o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
							o_Players[i]->getLookX(), o_Players[i]->getLookY()
						);
						randerer->drawHP(
							o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
							o_Players[i]->getFullHP(), o_Players[i]->getNowHP()
						);
					}
				}
			}
			for (auto b : items) {
				randerer->drawItem(b->getDrawX(), b->getDrawY());
			}
			for (auto b : bullets) {
				randerer->drawBullet(b->getDrawX(), b->getDrawY());
			}
			for (auto d : bushs)
				randerer->drawBush(d->getDrawX(), d->getDrawY(), d->getAlpha());
		}
		if (player_State == RESPAWN) {
			if (o_Players.size() > 0) {
				for (int i = 0; i < MAX_PLAYER - 1; ++i) {
					if (!o_Players[i]->hpZero()) {
						randerer->drawPlayer(
							o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
							1, 1, 1
						);
						randerer->drawO_BulletWay(
							o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
							o_Players[i]->getLookX(), o_Players[i]->getLookY()
						);
						randerer->drawHP(
							o_Players[i]->getDrawX(), o_Players[i]->getDrawY(),
							o_Players[i]->getFullHP(), o_Players[i]->getNowHP()
						);
					}
				}
			}
			for (auto b : items) {
				randerer->drawItem(b->getDrawX(), b->getDrawY());
			}
			for (auto b : bullets) {
				randerer->drawBullet(b->getDrawX(), b->getDrawY());
			}
			for (auto d : bushs)
				randerer->drawBush(d->getDrawX(), d->getDrawY(), d->getAlpha());
			int dec, sign;
			randerer->printtext(250, 250, fcvt(respawn_time, 0, &dec, &sign));
		}
	}
	
	if (killdeath_State) {
		if (m_Player != NULL) {
			char s[10];
			randerer->printtext(350, 430, "Kill     : ");
			randerer->printtext(350, 400, "Death : ");
			randerer->printtext(450, 430, itoa(m_Player->getKill(), s, 10));
			randerer->printtext(450, 400, itoa(m_Player->getDeath(), s, 10));
		}
	}
}

void SceneMgr::update() {
	int retval;
	int state;
	int frame_time;
	for (auto d : bushs)
		if (map != NULL)
			d->setDrawXY(map->getX(), map->getY());
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
		retval = send(server_sock, (char*)&player_State, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			break;
		}
		switch (player_State)
		{
		case START:
			retval = recv(server_sock, (char*)&frame_time, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			start_time -= ((float)frame_time / 1000.0f);
			retval = send(server_sock, (char*)&start_time, sizeof(float), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}
			break;
		case PLAY:
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
			retval = recvn(server_sock, (char*)&bullet_count, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&bulletsBuf, sizeof(BulletBuf)*bullet_count, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}

			retval = recvn(server_sock, (char*)&item_count, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&itemsBuf, sizeof(ItemBuf)*item_count, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}

			setPlayers();
			setBullets();
			setItems();
			break;
		case DIE:
			respawn_time = 5.0f;
			retval = recvn(server_sock, (char*)&playersBuf, PB_SIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&bullet_count, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&bulletsBuf, sizeof(BulletBuf)*bullet_count, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&item_count, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&itemsBuf, sizeof(ItemBuf)*item_count, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			setPlayers();
			setBullets();
			setItems();
			break;
		case RESPAWN:
			retval = recv(server_sock, (char*)&frame_time, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			respawn_time -= ((float)frame_time / 1000.0f);
			retval = send(server_sock, (char*)&respawn_time, sizeof(float), 0);
			if (retval == SOCKET_ERROR) {
				err_display("send()");
				break;
			}

			retval = recvn(server_sock, (char*)&playersBuf, PB_SIZE, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&bullet_count, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&bulletsBuf, sizeof(BulletBuf)*bullet_count, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&item_count, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			retval = recvn(server_sock, (char*)&itemsBuf, sizeof(ItemBuf)*item_count, 0);
			if (retval == SOCKET_ERROR) {
				err_display("recv()");
				break;
			}
			setPlayers();
			setBullets();
			setItems();
			break;
		}
		retval = recv(server_sock, (char*)&player_State, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		break;
	case END:
		break;
	}
	for (auto d : bushs) {
		if (m_Player != NULL) {
			if (collision(d, m_Player)) {
				d->hidingPlayer();
			}
			else
				d->outPlayer();
		}
	}
}

void SceneMgr::keyboardFunc(int key, int state) {
	if (player_State == DIE) {
		player_State = RESPAWN;
	}
	else {
		if (key == KEYBOARD_R) {
			if (player_State == WAIT)
				player_State = READY;
			else if (player_State == READY)
				player_State = WAIT;
		}
	}
	if (key == KEYBOARD_F) {
		if (state == 1)
			killdeath_State = true;
		else
			killdeath_State = false;
	}
}

void SceneMgr::initPlayersData() {
	m_Player = new Player(
		playersBuf[m_Player_Code].real_X, playersBuf[m_Player_Code].real_Y,
		playersBuf[m_Player_Code].hp
	);
	map->setXY(m_Player->getRealX(), m_Player->getRealY());
	for (int i = 1; i < MAX_PLAYER; ++i) {
		o_Players.push_back(
			new Player(
				playersBuf[m_Player_Code].real_X, playersBuf[m_Player_Code].real_Y,
				playersBuf[(m_Player_Code + i) % 3].real_X, playersBuf[(m_Player_Code + i) % 3].real_Y,
				playersBuf[(m_Player_Code + i) % 3].hp
			)
		);
	}
}

void SceneMgr::changeMove(int key, int state) {
	if (player_State == PLAY) {
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
}

void SceneMgr::setPlayers() {
	m_Player->update(
		playersBuf[m_Player_Code].real_X, playersBuf[m_Player_Code].real_Y,
		playersBuf[m_Player_Code].look_X, playersBuf[m_Player_Code].look_Y,
		playersBuf[m_Player_Code].hp
	);
	m_Player->setKD(
		playersBuf[m_Player_Code].kill,
		playersBuf[m_Player_Code].death
	);
	map->setXY(m_Player->getRealX(), m_Player->getRealY());

	for (int i = 1; i < MAX_PLAYER; ++i) {
		o_Players[i - 1]->o_Update(
			playersBuf[m_Player_Code].real_X, playersBuf[m_Player_Code].real_Y,
			playersBuf[(m_Player_Code + i) % 3].real_X, playersBuf[(m_Player_Code + i) % 3].real_Y,
			playersBuf[(m_Player_Code + i) % 3].look_X, playersBuf[(m_Player_Code + i) % 3].look_Y,
			playersBuf[(m_Player_Code + i) % 3].hp
		);
		o_Players[i - 1]->setKD(
			playersBuf[(m_Player_Code + i) % 3].kill,
			playersBuf[(m_Player_Code + i) % 3].death
		);
	}
}

void SceneMgr::setBullets() {
	bullets.clear();
	for (int i = 0; i < bullet_count; ++i) {
		bullets.push_back(
			new Bullet(
				map->getX(), map->getY(),
				bulletsBuf[i].real_X, bulletsBuf[i].real_Y
			));
	}
}


void SceneMgr::setItems() {
	items.clear();
	for (int i = 0; i < item_count; ++i) {
		items.push_back(
			new Item(
				map->getX(), map->getY(),
				itemsBuf[i].real_X, itemsBuf[i].real_Y
			));
	}
}

void SceneMgr::shootBullet() {
	if (m_Player != NULL)
		m_Player->shootBullet();
}

void SceneMgr::stopBullet() {
	if (m_Player != NULL)
		m_Player->stopBullet();
}