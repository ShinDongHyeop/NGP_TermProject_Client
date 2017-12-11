#pragma once

class Item {
	float draw_X, draw_Y;
public:
	Item(float mapX, float mapY, float realX, float realY);

	float getDrawX() { return draw_X; }
	float getDrawY() { return draw_Y; }
};