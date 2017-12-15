#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <gl/glut.h>
#include <math.h>
#include <string>
#include <iostream>
#include <vector>
#include <memory>
#include <time.h>
#include <algorithm>

using namespace std;

#define MAX_PLAYER		10
#define MAX_BULLET		100
#define PB_SIZE			(sizeof(PlayerBuf) * MAX_PLAYER)

#define MAP_SIZE		(2000 / 500)
#define PLAYER_SIZE		10.0f
#define BULLET_SIZE		3.0f
#define BUSH_SIZE		42
#define HP_BAR_SIZE		60

#define MAX_ITEM		5
#define ITEM_SIZE		8

#define NULL_PLAYER		999
#define ROOM_OWNER		0

#define PI				(3.14f / 180.0f)

enum ShootState { NO_SHOOT, SHOOT };
enum KeyboardState { KEYBOARD_A, KEYBOARD_S, KEYBOARD_D, KEYBOARD_W, KEYBOARD_R, KEYBOARD_F };
enum GameState { LOGIN, RUNNING, END };
enum PlayerState { NONE, WAIT, READY, START, PLAY, DIE };
enum StartOK { ON, OFF, STARTOK};
enum KillDeath { SEE, NOSEE };

template<class Object1, class Object2>
bool collision(Object1 object1, Object2 object2) {
	float* collBox1 = (*object1)->getCollBox();
	float* collBox2 = object2->getCollBox();
	if (collBox1[0] > collBox2[1])		return false;
	if (collBox1[1] < collBox2[0])		return false;
	if (collBox1[2] > collBox2[3])		return false;
	if (collBox1[3] < collBox2[2])		return false;
	return true;
}
