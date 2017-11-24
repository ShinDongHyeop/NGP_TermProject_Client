#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <vector>
#include <time.h>
#include "Randerer.h"
#include "Map.h"

using namespace std;


enum KeyboardState{KEYBOARD_A, KEYBOARD_S, KEYBOARD_D, KEYBOARD_W, KEYBOARD_R};
enum GameState{LOGIN, RUNNING, END};
enum PlayerState{WAIT, READY, START, PLAY, DIE, RESPAWN};


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
	Map* map = NULL;
	SOCKET server_sock;
	int retval;
	int game_State = LOGIN;
	int player_State = WAIT;
public:
	SceneMgr(LPVOID sock);

	~SceneMgr();

	void draw();

	void update(int frame_time);

	void keyboardFunc(int key, int state);

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

	// 소켓 함수 오류 출력
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
};