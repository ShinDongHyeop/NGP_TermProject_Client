#include <time.h>
#include "SceneMgr.h"

#define LEFT 0
#define RIGHT 500
#define TOP 500
#define BOTTOM 0
#define FRAME_TIME (int)(1000 / 60)

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 5000
#define BUFSIZE    50

using namespace std;

clock_t current_time = clock();
clock_t frame_time;

void draw(GLvoid);
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void Keyboardup(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
//void Timerfunction(int value);

SceneMgr* s;

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
	LocalFree(lpMsgBuf);
}

int main(int argc, char* argv[]) {
	int retval;

	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(RIGHT, TOP);
	glutCreateWindow("Example2");

	s = new SceneMgr((LPVOID)sock);

	glutDisplayFunc(draw);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Keyboardup);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(Motion);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void draw(GLvoid) {
	frame_time = clock() - current_time;
	current_time += frame_time;
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	s->update((int)frame_time);

	glutPostRedisplay();

	s->draw();

	glFlush();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(LEFT, RIGHT, BOTTOM, TOP, -1, 1);
}


void Keyboard(unsigned char key, int x, int y) {
	if (key == 'q')
		exit(1);
	if (key == 'r')
		s->keyboardFunc(KEYBOARD_R, 1);
	if (key == 'w')
		s->changeMove(KEYBOARD_W, 1);
	if (key == 'a')
		s->changeMove(KEYBOARD_A, 1);
	if (key == 's')
		s->changeMove(KEYBOARD_S, 1);
	if (key == 'd')
		s->changeMove(KEYBOARD_D, 1);
}

void Keyboardup(unsigned char key, int x, int y) {
	if (key == 'w')
		s->changeMove(KEYBOARD_W, -1);
	if (key == 'a')
		s->changeMove(KEYBOARD_A, -1);
	if (key == 's')
		s->changeMove(KEYBOARD_S, -1);
	if (key == 'd')
		s->changeMove(KEYBOARD_D, -1);
}

void Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		s->shootBullet();
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		s->stopBullet();
	}
}

void Motion(int x, int y) {
	s->mouseMotion(x, 500 - y);
}
