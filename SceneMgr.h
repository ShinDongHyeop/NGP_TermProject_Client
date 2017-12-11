#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <vector>
#include <time.h>
#include "Randerer.h"
#include "Map.h"
#include "Buffer.h"
#include "Player.h"
#include "Bullet.h"

using namespace std;

#define MAX_PLAYER		3
#define MAX_BULLET		100
#define PB_SIZE			(sizeof(PlayerBuf) * MAX_PLAYER)

enum KeyboardState{KEYBOARD_A, KEYBOARD_S, KEYBOARD_D, KEYBOARD_W, KEYBOARD_R, KEYBOARD_F};
enum GameState{LOGIN, RUNNING, END};
enum PlayerState{WAIT, READY, START, PLAY, DIE, RESPAWN};
enum KDState{ON, OFF};

template<class Object1, class Object2>
bool collision(Object1 object1, Object2 object2) {
	float* collBox1 = object1->getCollBox();
	float* collBox2 = object2->getCollBox();
	if (collBox1[0] > collBox2[1])		return false;
	if (collBox1[1] < collBox2[0])		return false;
	if (collBox1[2] > collBox2[3])		return false;
	if (collBox1[3] < collBox2[2])		return false;
	return true;
}

class SceneMgr {
	Randerer* randerer = NULL;
	Player* m_Player = NULL;
	int m_Player_Code;
	Map* map = NULL;
	SOCKET server_sock;
	vector<Player*> o_Players;
	vector<Bullet*> bullets;
	PlayerBuf playersBuf[3];
	BulletBuf bulletsBuf[MAX_BULLET];
	ClientBuf cb;
	int mMoveState[2]{};
	int bullet_count;
	int retval;
	int game_State = LOGIN;
	int player_State = WAIT;
	float start_time, respawn_time;
	bool killdeath_State = false;
public:
	SceneMgr(LPVOID sock);

	~SceneMgr();

	void draw();

	void update();

	void keyboardFunc(int key, int state);

	void mouseMotion(int x, int y) {
		if (m_Player != NULL)
			m_Player->setMyLookXY(x, y);
	}

	void initPlayersData();
	void changeMove(int key, int state);

	void setClientBuf() {
		int* state = m_Player->getMoveState();
		cb.move_State[0] = state[0];
		cb.move_State[1] = state[1];
		cb.shoot_State = m_Player->getShootState();
		cb.look_X = m_Player->getLookX();
		cb.look_Y = m_Player->getLookY();
	}

	void setPlayers();
	void setBullets();

	void shootBullet();
	void stopBullet();

	void err_quit(char *msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
		LocalFree(lpMsgBuf);
		exit(1);
	}

	void err_display(char *msg)
	{
		LPVOID lpMsgBuf;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		printf("[%s] %s", msg, (char *)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}

	int recvn(SOCKET s, char *buf, int len, int flags)
	{
		int received;
		char *ptr = buf;
		int left = len;

		while (left > 0) {
			received = recv(s, ptr, left, flags);
			if (received == SOCKET_ERROR)
				return SOCKET_ERROR;
			else if (received == 0)
				break;
			left -= received;
			ptr += received;
		}

		return (len - left);
	}
};