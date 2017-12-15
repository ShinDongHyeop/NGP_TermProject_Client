#include "Bush_Object.h"
#include "support.h"

Bush::Bush(float bX, float bY, float mapX, float mapY) {
	real_X = bX;		real_Y = bY;
	draw_X = real_X - mapX;
	draw_Y = real_Y - mapY;
	collBox[0] = real_X - BUSH_SIZE;
	collBox[1] = real_X + BUSH_SIZE;
	collBox[2] = real_Y - BUSH_SIZE;
	collBox[3] = real_Y + BUSH_SIZE;
	alpha = 1.0f;
}

void Bush::setDrawXY(float mapX, float mapY) {
	draw_X = real_X - mapX;
	draw_Y = real_Y - mapY;
}