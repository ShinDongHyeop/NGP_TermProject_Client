#include "Map_Object.h"

Map::Map(float pRealX, float pRealY) :
	x(pRealX - 250), y(pRealY - 250) {}

void Map::setXY(float pRealX, float pRealY) {
	if ((250 < pRealX) && (pRealX <= 1750))
		x = pRealX - 250;
	if (pRealX <= 250)
		x = 0;
	if (pRealX > 1750)
		x = 1500;

	if ((250 < pRealY) && (pRealY <= 1750))
		y = pRealY - 250;
	if (pRealY <= 250)
		y = 0;
	if (pRealY > 1750)
		y = 1500;
}