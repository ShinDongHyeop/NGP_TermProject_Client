#include "Bullet_Object.h"
#include "support.h"

Bullet::Bullet(float mapX, float mapY, float realX, float realY) {
	draw_X = realX - mapX;
	draw_Y = realY - mapY;
}