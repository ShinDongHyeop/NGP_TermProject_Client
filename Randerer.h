#include <gl/glut.h>
#include <math.h>
#include <string>


#define MAP_SIZE (2000 / 500)
#define PLAYER_SIZE 10.0f
#define BULLET_SIZE 3.0f
#define BUSH_SIZE  42

class Randerer {
public:
	void drawPlayer(float x, float y, float r, float g, float b) {
		glColor3f(r, g, b);
		glRectf(
			x - PLAYER_SIZE, y - PLAYER_SIZE,
			x + PLAYER_SIZE, y + PLAYER_SIZE
		);
	}

	void drawHP(float x, float y, float fullHP, float nowHP) {
		glColor3f(0.0f, 0.0f, 0.0f);
		glRectf(x - (fullHP / 2), y + 15, x + (fullHP / 2), y + 20);
		glColor3f(1.0f, 0.0f, 0.0f);
		glRectf(x - (fullHP / 2), y + 15, x - (fullHP / 2) + nowHP, y + 20);
	}

	void drawBullet(float x, float y) {
		glColor3f(0.0f, 1.0f, 1.0f);
		glRectf(
			x - BULLET_SIZE, y - BULLET_SIZE,
			x + BULLET_SIZE, y + BULLET_SIZE
		);
	}

	void printtext(int x, int y, char* s)
	{
		glPushMatrix();
		glPushAttrib(GL_DEPTH_TEST);
		glColor3f(1.0f, 0.0f, 0.0f);
		glRasterPos2i(x, y);
		for (int i = 0; i<(int)strlen(s); i++)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
		}
		glPopAttrib();
		glPopMatrix();
	}

	void drawMap(float x, float y) {
		for (int i = 0; i < (MAP_SIZE * 2); ++i) {
			for (int j = 0; j < (MAP_SIZE * 2); ++j) {
				if ((i + j) % 2 == 0) {
					glColor3f(0.8f, 0.65f, 0.24f);
				}
				else {
					glColor3f(0.95f, 0.59f, 0.38f);
				}
				glRectf(
					(i * 250) - x,
					(j * 250) - y,
					((i + 1) * 250) - x,
					((j + 1) * 250) - y
				);
			}
		}
	}

	void drawBulletWay(float x, float y, float sx, float sy) {
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
		glBegin(GL_POLYGON);
		glVertex2f(x + (5 * sy) + (10 * sx), y - (5 * sx) + (10 * sy));
		glVertex2f(x + (50 * sx), y + (50 * sy));
		glVertex2f(x - (5 * sy) + (10 * sx), y + (5 * sx) + (10 * sy));
		glEnd();
	}

	void drawBush(float x, float y, float a) {
		glPushMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0.0f, 0.8f, 0.0f, a);
		glRectf(
			x - BUSH_SIZE, y - BUSH_SIZE,
			x + BUSH_SIZE, y + BUSH_SIZE
		);
		glDisable(GL_BLEND);

		glPopMatrix();
	}
};