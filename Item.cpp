#include "Item.h"

Item::Item(float mapX, float mapY, float realX, float realY) {
	draw_X = realX - mapX;
	draw_Y = realY - mapY;
}