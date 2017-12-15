class Item {
	float real_X, real_Y;
	float draw_X, draw_Y;
	int state;
public:
	Item(int s, float realX, float realY, float mapX, float mapY);

	float getRealX() { return real_X; }
	float getRealY() { return real_Y; }
	float getDrawX() { return draw_X; }
	float getDrawY() { return draw_Y; }
	int getState() { return state; }

	void setDrawXY(int s, float realX, float realY, float mapX, float mapY);
};