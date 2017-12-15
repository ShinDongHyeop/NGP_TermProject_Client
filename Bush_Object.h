class Bush {
	float real_X, real_Y;
	float draw_X, draw_Y;
	float alpha;
	float collBox[4];
public:
	Bush(float bX, float bY, float mapX, float mapY);
	
	float getRealX() { return real_X; }
	float getRealY() { return real_Y; }
	float getDrawX() { return draw_X; }
	float getDrawY() { return draw_Y; }
	float getAlpha() { return alpha; }
	float* getCollBox() { return collBox; }

	void hidingPlayer() { alpha = 0.3f; }
	void outPlayer() { alpha = 1.0f; }

	void setDrawXY(float mapX, float mapY);
};