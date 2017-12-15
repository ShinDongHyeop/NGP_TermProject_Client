#include "SceneMgr.h"

SceneMgr::SceneMgr(LPVOID sock) {
	server_sock = (SOCKET)sock; 
	BOOL opt_val = TRUE;
	setsockopt(server_sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));

	map = new Map(1000, 1000);
	retval = recv(server_sock, (char*)&m_Player_Code, sizeof(int), 0);
	if (retval == SOCKET_ERROR) { err_display("recv()");	closesocket(server_sock);	 return; }
	game_State = LOGIN;
	frame_time = 0;
	ready_State = 0;
	start_State = OFF;
	game_Start_time = 0;
	respawn_time = 0;
	killdeathUI = NOSEE;
	all_Count[0] = all_Count[1] = 0;
	m_Player = NULL;
	winner = NULL_PLAYER;
	room_owner = ROOM_OWNER;
	m_Move_State[0] = m_Move_State[1] = 0;
	for (int i = 0; i < MAX_PLAYER; ++i)
		players_State[i] = NONE;
}

SceneMgr::~SceneMgr() {
	delete map;
	delete renderer;
	delete m_Player;
	closesocket(server_sock);
}

void SceneMgr::initBush() {
	bushs.clear();
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 8; ++j)
			bushs.push_back(
				unique_ptr<Bush>(new Bush(
					i * 100 + 200, j * 100 + 1000,
					map->getX(), map->getY()
				))
			);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			bushs.push_back(
				unique_ptr<Bush>(new Bush(
					i * 100 + 850, j * 100 + 1100,
					map->getX(), map->getY()
				))
			);

	for (int i = 0; i < 8; ++i)
		for (int j = 0; j < 2; ++j)
			bushs.push_back(
				unique_ptr<Bush>(new Bush(
					i * 100 + 1300, j * 100 + 1750,
					map->getX(), map->getY()
				))
			);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 6; ++j)
			bushs.push_back(
				unique_ptr<Bush>(new Bush(
					i * 100 + 600, j * 100 + 200,
					map->getX(), map->getY()
				))
			);

	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 6; ++j)
			bushs.push_back(
				unique_ptr<Bush>(new Bush(
					i * 100 + 1400, j * 100 + 200,
					map->getX(), map->getY()
				))
			);
}

void SceneMgr::initPlayerData() {
	if (m_Player != NULL) {
		m_Player = NULL;
		
	}
	if (o_Players.size() > 0)
		o_Players.clear();

	for (int i = 0; i < all_Count[0]; ++i) {
		if (players_Buf[i].id == m_Player_Code) {
			m_Player = new Player(
				players_Buf[i].id, 
				players_Buf[i].real_X, 
				players_Buf[i].real_Y, 
				players_Buf[i].hp
			);
			map->setXY(m_Player->getRealX(), m_Player->getRealY());
			initBush();
			break;
		}
	}
	for (int i = 0; i < all_Count[0]; ++i) {
		if (players_Buf[i].id == m_Player_Code) {
			continue;
		}
		else {
			o_Players.push_back(
				unique_ptr<Player>(
					new Player(
						m_Player->getRealX(), m_Player->getRealY(),
						players_Buf[i].id, 
						players_Buf[i].real_X, 
						players_Buf[i].real_Y, 
						players_Buf[i].hp
					)
				)
			);
		}
	}
}

void SceneMgr::setClientBuf() {
	if (m_Player != NULL) {
		int* ms = m_Player->getMoveState();
		client_Buf.look_X = m_Player->getLookX();
		client_Buf.look_Y = m_Player->getLookY();
		client_Buf.shoot_State = m_Player->getShootState();
		client_Buf.move_State[0] = ms[0];
		client_Buf.move_State[1] = ms[1];
	}
}

bool SceneMgr::idInPlayersCheck(int id) {
	for (auto player = o_Players.begin(); player != o_Players.end(); ++player) {
		if ((*player)->getID() == id)
			return false;
	}
	return true;
}

void SceneMgr::setPlayerData() {
	if (m_Player != NULL) {
		for (int i = 0; i < all_Count[0]; ++i) {
			if (players_Buf[i].id == m_Player_Code) {
				m_Player->update(
					players_Buf[i].real_X,
					players_Buf[i].real_Y,
					players_Buf[i].hp
				);
				m_Player->setKD(players_Buf[i].kill, players_Buf[i].death);
				map->setXY(m_Player->getRealX(), m_Player->getRealY());
				for (auto bush = bushs.begin(); bush != bushs.end(); ++bush) {
					(*bush)->setDrawXY(map->getX(), map->getY());
				}
				break;
			}
		}
		for (int i = 0; i < all_Count[0]; ++i) {
			PlayerBuf pb = players_Buf[i];
			if (pb.id == m_Player_Code)
				continue;
			auto player = find_if(o_Players.begin(), o_Players.end(), [pb](unique_ptr<Player>& a) {
				return a->getID() == pb.id;
			});
			(*player)->o_Update(
				m_Player->getRealX(), m_Player->getRealY(),
				pb.real_X, pb.real_Y,
				pb.look_X, pb.look_Y,
				pb.hp
			);
			(*player)->setKD(pb.kill, pb.death);
		}
	}
}

void SceneMgr::setBulletData() {
	bullets.clear();
	for (int i = 0; i < all_Count[1]; ++i) {
		bullets.push_back(
			unique_ptr<Bullet>(new Bullet(
				map->getX(), map->getY(),
				bullets_Buf[i].real_X,
				bullets_Buf[i].real_Y
			))
		);
	}
}

void SceneMgr::initItemData() {
	for (int i = 0; i < MAX_ITEM; ++i) {
		items.push_back(
			unique_ptr<Item>(new Item(
				items_Buf[i].state,
				items_Buf[i].realX, items_Buf[i].realY,
				map->getX(), map->getY()
			))
		);
	}
}

void SceneMgr::setItemData() {
	int num = 0;
	for (auto i = items.begin(); i != items.end(); ++i) {
		(*i)->setDrawXY(items_Buf[num].state, items_Buf[num].realX, items_Buf[num].realY, map->getX(), map->getY());
		++num;
	}
}

void SceneMgr::shootBullet() {
	if(m_Player != NULL && players_State[m_Player_Code] == PLAY)
		m_Player->shootBullet();
}

void SceneMgr::stopBullet() {
	if (m_Player != NULL && players_State[m_Player_Code] == PLAY)
		m_Player->stopBullet();
}

void SceneMgr::draw() {
	char buf[10];
	if (map != NULL)
		renderer->drawMap(
			map->getX(), map->getY()
		);
	switch (game_State)
	{
	case LOGIN:
		for (int i = 0; i < MAX_PLAYER; ++i) {
			if (i == m_Player_Code)
				renderer->printtext(50, 400 - (i * 30), "ME->");
			renderer->printtext(100, 400 - (i * 30), "Player");
			if (i == room_owner) {
				renderer->printtext(180, 400 - (i * 30), itoa(i, buf, 10));
				renderer->printtext(250, 400 - (i * 30), "ROOM OWNER");
			}
			else {
				renderer->printtext(180, 400 - (i * 30), itoa(i, buf, 10));
				if (players_State[i] == WAIT)
					renderer->printtext(292, 400 - (i * 30), "WAIT");
				if (players_State[i] == READY)
					renderer->printtext(287, 400 - (i * 30), "READY");
				if (players_State[i] == NONE)
					renderer->printtext(290, 400 - (i * 30), "NONE");
			}
		}
		if (start_State == ON) {
			renderer->printtext(200, 70, "ALL READY!");
			renderer->printtext(80, 50, "ROOM OWNER : R BUTTON TO START");
		}
		break;
	case RUNNING:
		switch (players_State[m_Player_Code])
		{
		case START:
			renderer->printtext(250, 250, itoa(5 - (game_Start_time / 1000), buf, 10));
			renderer->printtext(180, 200, "To Start The Game");
			break;
		case PLAY:
			if (m_Player != NULL) {
				renderer->drawPlayer(m_Player->getDrawX(), m_Player->getDrawY(), 1.0f, 1.0f, 1.0f);
				renderer->drawBulletWay(m_Player->getDrawX(), m_Player->getDrawY(), m_Player->getLookX(), m_Player->getLookY());
				renderer->drawHP(m_Player->getDrawX(), m_Player->getDrawY(), m_Player->getFullHP(), m_Player->getNowHP());
			}
			if (o_Players.size() > 0) {
				for (auto i = o_Players.begin(); i != o_Players.end(); ++i) {
					if (players_State[(*i)->getID()] != DIE && players_State[(*i)->getID()] != NONE) {
						renderer->drawPlayer((*i)->getDrawX(), (*i)->getDrawY(), 1.0f, 1.0f, 1.0f);
						renderer->drawO_BulletWay((*i)->getDrawX(), (*i)->getDrawY(), (*i)->getLookX(), (*i)->getLookY());
						renderer->drawHP((*i)->getDrawX(), (*i)->getDrawY(), (*i)->getFullHP(), (*i)->getNowHP());
					}
				}
			}
			if (bullets.size() > 0) {
				for (auto i = bullets.begin(); i != bullets.end(); ++i) {
					renderer->drawBullet((*i)->getDrawX(), (*i)->getDrawY());
				}
			}
			for (auto item = items.begin(); item != items.end(); ++item) {
				if((*item)->getState() != 1)
					renderer->drawItem((*item)->getDrawX(), (*item)->getDrawY());
			}
			for (auto bush = bushs.begin(); bush != bushs.end(); ++bush) {
				renderer->drawBush((*bush)->getDrawX(), (*bush)->getDrawY(), (*bush)->getAlpha());
			}
			if (m_Player != NULL) {
				renderer->printtext(350, 450, "KILL");
				renderer->printtext(350, 420, "DEATH");
				renderer->printtext(430, 450, ":");
				renderer->printtext(430, 420, ":");
				renderer->printtext(450, 450, itoa(m_Player->getKill(), buf, 10));
				renderer->printtext(450, 420, itoa(m_Player->getDeath(), buf, 10));
			}
			if (killdeathUI == SEE) {
				int num = 0;
				renderer->printtext(60, 380, "Player Name");
				renderer->printtext(235, 380, "KILL");
				renderer->printtext(325, 380, "DEATH");
				for (auto i = o_Players.begin(); i != o_Players.end(); ++i) {
					if (players_State[(*i)->getID()] != NONE) {
						renderer->printtext(70, 340 - (num * 30), "PLAYER");
						renderer->printtext(150, 340 - (num * 30), itoa((*i)->getID(), buf, 10));
						renderer->printtext(250, 340 - (num * 30), itoa((*i)->getKill(), buf, 10));
						renderer->printtext(350, 340 - (num * 30), itoa((*i)->getDeath(), buf, 10));
						++num;
					}
				}
				for (int i = num; i < MAX_PLAYER; ++i) {
					renderer->printtext(90, 340 - (i * 30), "NONE");
				}
			}
			break;
		case DIE:
			renderer->printtext(250, 250, itoa(5 - (respawn_time / 1000), buf, 10));
			for (auto i = o_Players.begin(); i != o_Players.end(); ++i) {
				if (players_State[(*i)->getID()] != DIE) {
					renderer->drawPlayer((*i)->getDrawX(), (*i)->getDrawY(), 1.0f, 1.0f, 1.0f);
					renderer->drawBulletWay((*i)->getDrawX(), (*i)->getDrawY(), (*i)->getLookX(), (*i)->getLookY());
					renderer->drawHP((*i)->getDrawX(), (*i)->getDrawY(), (*i)->getFullHP(), (*i)->getNowHP());
				}
			}
			for (auto i = bullets.begin(); i != bullets.end(); ++i) {
				renderer->drawBullet((*i)->getDrawX(), (*i)->getDrawY());
			}
			break;
		}
		break;
	case END:
		renderer->printtext(250, 300, itoa(5 - (restart_time / 1000), buf, 10));
		renderer->printtext(190, 270, "Restart Game");
		renderer->printtext(210, 230, "WINNER!!");
		renderer->printtext(210, 200, "PLAYER");
		renderer->printtext(290, 200, itoa(winner, buf, 10));
		break;
	}
}

void SceneMgr::update() {
	int pbsize, bbsize, len;
	retval = recv(server_sock, (char*)&room_owner, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		closesocket(server_sock);
		err_quit("recv()");
	}
	retval = recv(server_sock, (char*)&game_State, sizeof(int), 0);
	if (retval == SOCKET_ERROR) {
		closesocket(server_sock);
		err_quit("recv()");
	}
	len = (int)sizeof(int)*MAX_PLAYER;
	retval = recv(server_sock, (char*)&players_State, len, 0);
	if (retval == SOCKET_ERROR) {
		closesocket(server_sock);
		err_quit("recv()");
	}
	switch (game_State)
	{
	case LOGIN:
		retval = send(server_sock, (char*)&ready_State, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			closesocket(server_sock);
			err_quit("send()");
		}
		retval = recv(server_sock, (char*)&start_State, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			closesocket(server_sock);
			err_quit("recv()");
		}
		if (start_State == STARTOK) {
			retval = recv(server_sock, (char*)&all_Count[0], sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			pbsize = (int)sizeof(PlayerBuf) * all_Count[0];
			retval = recvn(server_sock, (char*)&players_Buf, pbsize, 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			len = (int)sizeof(ItemBuf)*MAX_ITEM;
			retval = recvn(server_sock, (char*)&items_Buf, len, 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			initPlayerData();
			initItemData();
		}
		break;
	case RUNNING:
		switch (players_State[m_Player_Code])
		{
		case START:
			ready_State = 0;
			retval = recv(server_sock, (char*)&game_Start_time, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			break;
		case PLAY:
			setClientBuf();
			retval = send(server_sock, (char*)&client_Buf, sizeof(ClientBuf), 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("send()");
			}
			retval = recv(server_sock, (char*)&all_Count, sizeof(all_Count), 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			pbsize = (int)sizeof(PlayerBuf) * all_Count[0];
			retval = recvn(server_sock, (char*)&players_Buf, pbsize, 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			bbsize = (int)sizeof(BulletBuf)*all_Count[1];
			retval = recvn(server_sock, (char*)&bullets_Buf, bbsize, 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			len = (int)sizeof(ItemBuf)*MAX_ITEM;
			retval = recvn(server_sock, (char*)&items_Buf, len, 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			setPlayerData();
			setBulletData();
			setItemData();
			for (auto bush = bushs.begin(); bush != bushs.end(); ++bush) {
				if (m_Player != NULL) {
					if (collision(bush, m_Player))
						(*bush)->hidingPlayer();
					else
						(*bush)->outPlayer();
				}
			}
			break;
		case DIE:
			retval = recv(server_sock, (char*)&respawn_time, sizeof(int), 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			retval = recv(server_sock, (char*)&all_Count, sizeof(all_Count), 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			pbsize = (int)sizeof(PlayerBuf) * all_Count[0];
			retval = recvn(server_sock, (char*)&players_Buf, pbsize, 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			bbsize = (int)sizeof(BulletBuf)*all_Count[1];
			retval = recvn(server_sock, (char*)&bullets_Buf, bbsize, 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			len = (int)sizeof(ItemBuf)*MAX_ITEM;
			retval = recvn(server_sock, (char*)&items_Buf, len, 0);
			if (retval == SOCKET_ERROR) {
				closesocket(server_sock);
				err_quit("recv()");
			}
			setPlayerData();
			setBulletData();
			setItemData();
			break;
		}
		break;
	case END:
		retval = recv(server_sock, (char*)&winner, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			closesocket(server_sock);
			err_quit("recv()");
		}
		retval = recv(server_sock, (char*)&restart_time, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			closesocket(server_sock);
			err_quit("recv()");
		}
		break;
	}
}

void SceneMgr::keyboardFunc(int key, int state) {
	if (key == KEYBOARD_R) {
		if (m_Player_Code == room_owner && game_State == LOGIN) {
			if (start_State == ON)
				if (ready_State == 0)
					ready_State = 1;
		}
		else {
			switch (ready_State)
			{
			case 0:
				ready_State = 1;
				break;
			case 1:
				ready_State = 0;
				break;
			}
		}
	}
	if (key == KEYBOARD_F) {
		if (game_State == RUNNING && players_State[m_Player_Code] != START) {
			switch (state) {
			case 1:
				killdeathUI = SEE;
				break;
			case -1:
				killdeathUI = NOSEE;
				break;
			}
		}
	}
}

void SceneMgr::changeMove(int key, int state) {
	if (game_State == RUNNING && players_State[m_Player_Code] == PLAY) {
		if (key == KEYBOARD_A) {
			if (state == 1) {
				if (m_Move_State[0] == 0)
					m_Move_State[0] = -1;
				else if (m_Move_State[0] == 1)
					m_Move_State[0] = 0;
			}
			else
				m_Move_State[0] -= state;
		}

		if (key == KEYBOARD_D) {
			if (state == 1) {
				if (m_Move_State[0] == 0)
					m_Move_State[0] = 1;
				else if (m_Move_State[0] == -1)
					m_Move_State[0] = 0;
			}
			else
				m_Move_State[0] += state;
		}

		if (key == KEYBOARD_W) {
			if (state == 1) {
				if (m_Move_State[1] == 0)
					m_Move_State[1] = 1;
				else if (m_Move_State[1] == -1)
					m_Move_State[1] = 0;
			}
			else
				m_Move_State[1] += state;
		}

		if (key == KEYBOARD_S) {
			if (state == 1) {
				if (m_Move_State[1] == 0)
					m_Move_State[1] = -1;
				else if (m_Move_State[1] == 1)
					m_Move_State[1] = 0;
			}
			else
				m_Move_State[1] -= state;
		}
		if (m_Player != NULL)
			m_Player->change_move(m_Move_State);
	}
}

void SceneMgr::mouseMotion(int x, int y) {
	if (m_Player != NULL && players_State[m_Player_Code] == PLAY)
		m_Player->setMyLookXY(x, y);
}