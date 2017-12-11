#pragma once
#define BUSH_SIZE  42

class Bush {
	float x, y;
	float drawX, drawY;
	float alpha;
	float collBox[4];
public:
	Bush(float bX, float bY, float mapX, float mapY) {
		x = bX;		y = bY;
		drawX = x - mapX;
		drawY = y - mapY;
		collBox[0] = x - BUSH_SIZE;
		collBox[1] = x + BUSH_SIZE;
		collBox[2] = y - BUSH_SIZE;
		collBox[3] = y + BUSH_SIZE;
		alpha = 1.0f;
	}
	void setDrawXY(float mapX, float mapY) {
		drawX = x - mapX;
		drawY = y - mapY;
	}

	float getX() { return x; }
	float getY() { return y; }
	float getDrawX() { return drawX; }
	float getDrawY() { return drawY; }
	float getAlpha() { return alpha; }
	float* getCollBox() { return collBox; }

	void hidingPlayer() { alpha = 0.3f; }
	void outPlayer() { alpha = 1.0f; }
}; 
