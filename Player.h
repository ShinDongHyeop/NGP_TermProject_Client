
enum ShootState {NO_SHOOT, SHOOT};

class Player {
	int state;
	int move_State[2]{};
	int shoot_State;
	float full_HP, now_HP;
	float real_X, real_Y;
	float draw_X, draw_Y;
	float look_X, look_Y;
public:
	Player(float realX, float realY, float hp);
	Player(float m_realX, float m_realY, float realX, float realY, float hp);

	int getState() { return state; }
	int* getMoveState() { return move_State; }
	int getShootState() { return shoot_State; }
	float getDrawX() { return draw_X; }
	float getDrawY() { return draw_Y; }
	float getRealX() { return real_X; }
	float getRealY() { return real_Y; }
	float getLookX() { return look_X; }
	float getLookY() { return look_Y; }
	float getFullHP() { return full_HP; }
	float getNowHP() { return now_HP; }
	
	float setXY(float realXY);

	void change_move(int* state);
	void setOtherLookXY(float lookX, float lookY);
	void setMyLookXY(float mouseX, float mouseY);
	void shootBullet();
	void stopBullet();

	void update(float realX, float realY, float lookX, float lookY, float hp);
	void o_Update(float m_realX, float m_realY, float realX, float realY, float lookX, float lookY, float hp);
};