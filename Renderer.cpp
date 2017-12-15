#include "Renderer.h"
#include "support.h"

void Renderer::drawPlayer(float x, float y, float r, float g, float b) {
	glColor3f(r, g, b);
	glRectf(
		x - PLAYER_SIZE, y - PLAYER_SIZE,
		x + PLAYER_SIZE, y + PLAYER_SIZE
	);
}

void Renderer::drawHP(float x, float y, float fullHP, float nowHP) {
	float num = nowHP / fullHP * HP_BAR_SIZE;
	glColor3f(0.0f, 0.0f, 0.0f);
	glRectf(x - (HP_BAR_SIZE / 2), y + 15, x + (HP_BAR_SIZE / 2), y + 20);
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(x - (HP_BAR_SIZE / 2), y + 15, x - (HP_BAR_SIZE / 2) + num, y + 20);
}

void Renderer::drawBullet(float x, float y) {
	glColor3f(0.0f, 1.0f, 1.0f);
	glRectf(
		x - BULLET_SIZE, y - BULLET_SIZE,
		x + BULLET_SIZE, y + BULLET_SIZE
	);
}

void Renderer::printtext(int x, int y, char* s)
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

void Renderer::drawMap(float x, float y) {
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

void Renderer::drawBulletWay(float x, float y, float sx, float sy) {
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_POLYGON);
	glVertex2f(x + (5 * sy) + (10 * sx), y - (5 * sx) + (10 * sy));
	glVertex2f(x + (50 * sx), y + (50 * sy));
	glVertex2f(x - (5 * sy) + (10 * sx), y + (5 * sx) + (10 * sy));
	glEnd();
}

void Renderer::drawO_BulletWay(float x, float y, float sx, float sy) {
	glColor4f(1.0f, 0.0f, 0.0f, 0.5f);
	glBegin(GL_POLYGON);
	glVertex2f(x + (5 * sy) + (10 * sx), y - (5 * sx) + (10 * sy));
	glVertex2f(x + (30 * sx), y + (30 * sy));
	glVertex2f(x - (5 * sy) + (10 * sx), y + (5 * sx) + (10 * sy));
	glEnd();
}

void Renderer::drawBush(float x, float y, float a) {
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

void Renderer::drawItem(float x, float y) {
	glColor3f(1.0f, 0.0f, 0.0f);
	glRectf(x - ITEM_SIZE, y - ITEM_SIZE, x + ITEM_SIZE, y + ITEM_SIZE);
	glColor3f(1.0f, 1.0f, 1.0f);
	glRectf(x - (ITEM_SIZE / 4), y - (ITEM_SIZE / 2), x + (ITEM_SIZE / 4), y + (ITEM_SIZE / 2));
	glRectf(x - (ITEM_SIZE / 2), y - (ITEM_SIZE / 4), x + (ITEM_SIZE / 2), y + (ITEM_SIZE / 4));
}