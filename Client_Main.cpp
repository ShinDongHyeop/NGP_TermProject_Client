#include "SceneMgr.h"

#define WIDTH		500
#define HEGHIT		500
#define SERVERIP	"127.0.0.1"
#define SERVERPORT	5000

void draw(GLvoid);
void idle(void);
void Reshape(int w, int h);
void Keyboard(unsigned char key, int x, int y);
void Keyboardup(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

SceneMgr* s;

int main(int argc, char* argv[]) {
	int retval;
	glutInit(&argc, argv);
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) s->err_quit("socket()");
	BOOL opt_val = TRUE;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char*)&opt_val, sizeof(opt_val));
	char serverip[15];
	cout << "Server IP : ";
	cin >> serverip;

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(serverip);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) s->err_quit("connect()");

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEGHIT);
	glutCreateWindow("Example2");

	s = new SceneMgr((LPVOID)sock);

	glutDisplayFunc(draw);
	glutIdleFunc(idle);
	glutKeyboardFunc(Keyboard);
	glutKeyboardUpFunc(Keyboardup);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(Motion);
	glutMotionFunc(Motion);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

void draw(GLvoid) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	s->update();

	s->draw();

	glutSwapBuffers();
}

void idle(void) {
	draw();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glOrtho(0, WIDTH, 0, HEGHIT, -1, 1);
}


void Keyboard(unsigned char key, int x, int y) {
	if (key == 'q')
		exit(1);
	if (key == 'r')
		s->keyboardFunc(KEYBOARD_R, 1);
	if (key == 'f')
		s->keyboardFunc(KEYBOARD_F, 1);
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
	if (key == 'f')
		s->keyboardFunc(KEYBOARD_F, -1);
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
