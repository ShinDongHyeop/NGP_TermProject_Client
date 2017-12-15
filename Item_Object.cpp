#include "Item_Object.h"
#include "support.h"

Item::Item(int s, float realX, float realY, float mapX, float mapY) {
	real_X = realX;			real_Y = realY;
	draw_X = real_X - mapX;
	draw_Y = real_Y - mapY;
	state = s;
}

void Item::setDrawXY(int s, float realX, float realY, float mapX, float mapY) {
	real_X = realX;			real_Y = realY;
	draw_X = real_X - mapX;
	draw_Y = real_Y - mapY;
	state = s;
}