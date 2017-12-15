class Renderer {
public:
	void drawPlayer(float x, float y, float r, float g, float b);

	void drawHP(float x, float y, float fullHP, float nowHP);

	void drawBullet(float x, float y);

	void printtext(int x, int y, char* s);

	void drawMap(float x, float y);

	void drawBulletWay(float x, float y, float sx, float sy);

	void drawO_BulletWay(float x, float y, float sx, float sy);

	void drawBush(float x, float y, float a);

	void drawItem(float x, float y);
};