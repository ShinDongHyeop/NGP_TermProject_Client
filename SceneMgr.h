#include "Player_Object.h"
#include "Buffer_Object.h"
#include "Bullet_Object.h"
#include "Item_Object.h"
#include "Bush_Object.h"
#include "Map_Object.h"
#include "Renderer.h"
#include "support.h"

class SceneMgr {
	SOCKET server_sock;
	Renderer* renderer;
	Map* map;
	Player* m_Player;
	vector<unique_ptr<Player>> o_Players;
	vector<unique_ptr<Bullet>> bullets;
	vector<unique_ptr<Bush>> bushs;
	vector<unique_ptr<Item>> items;
	PlayerBuf players_Buf[MAX_PLAYER];
	BulletBuf bullets_Buf[MAX_BULLET];
	ClientBuf client_Buf;
	ItemBuf items_Buf[MAX_ITEM];
	int m_Player_Code;
	int game_State;
	int players_State[MAX_PLAYER];
	int start_State;
	int ready_State;
	int killdeathUI;
	int retval;
	clock_t frame_time;
	int game_Start_time;
	int respawn_time;
	int restart_time;
	int winner;
	int m_Move_State[2];
	int all_Count[2];
	int room_owner;
public:
	SceneMgr(LPVOID sock);
	~SceneMgr();

	bool idInPlayersCheck(int id);
	void initBush();
	void initPlayerData();
	void initItemData();
	void setPlayerData();
	void setBulletData();
	void setItemData();
	void setClientBuf();
	void shootBullet();
	void stopBullet();

	void draw();
	void update();
	void keyboardFunc(int key, int state);
	void changeMove(int key, int state);
	void mouseMotion(int x, int y);

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