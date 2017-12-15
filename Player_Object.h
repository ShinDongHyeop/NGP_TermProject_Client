class Player {
	int state, id;
	int move_State[2]{};
	int shoot_State;
	int kill, death;
	float full_HP, now_HP;
	float real_X, real_Y;
	float draw_X, draw_Y;
	float look_X, look_Y;
	float coll_Box[4];
public:
	Player(int code, float realX, float realY, float hp);
	Player(float m_realX, float m_realY, int code, float realX, float realY, float hp);

	int getState() { return state; }
	int getID() { return id; }
	int* getMoveState() { return move_State; }
	int getShootState() { return shoot_State; }
	int getKill() { return kill; }
	int getDeath() { return death; }
	float getDrawX() { return draw_X; }
	float getDrawY() { return draw_Y; }
	float getRealX() { return real_X; }
	float getRealY() { return real_Y; }
	float getLookX() { return look_X; }
	float getLookY() { return look_Y; }
	float getFullHP() { return full_HP; }
	float getNowHP() { return now_HP; }
	float setXY(float realXY);
	float* getCollBox() { return coll_Box; }

	bool hpZero() { return now_HP <= 0; }

	void change_move(int* state);
	void setOtherLookXY(float lookX, float lookY);
	void setMyLookXY(float mouseX, float mouseY);
	void shootBullet();
	void stopBullet();
	void setKD(int pKill, int pDeath) {
		kill = pKill;		death = pDeath;
	}

	void update(float realX, float realY, float hp);
	void o_Update(float m_realX, float m_realY, float realX, float realY, float lookX, float lookY, float hp);
};