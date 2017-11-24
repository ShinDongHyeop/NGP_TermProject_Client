#include <math.h>
#include <stdio.h>
#include "Player.h"

Player::Player(float realX, float realY, float hp) {
	real_X = realX;		real_Y = realY;
	full_HP = hp;		now_HP = hp;
	draw_X = setXY(realX);
	draw_Y = setXY(realY);
}

Player::Player(float m_realX, float m_realY, float realX, float realY, float hp) {
	float drawX = setXY(m_realX) - m_realX;
	float drawY = setXY(m_realY) - m_realY;
	real_X = realX;		real_Y = realY;
	full_HP = hp;		now_HP = hp;
	draw_X = realX - drawX;
	draw_Y = realY - drawY;
}

float Player::setXY(float realXY) {
	if (realXY < 250.0f)
		return realXY;
	if (realXY > 1750.0f)
		return (realXY - 1500.0f);
	return 250.0f;
}

void Player::setOtherLookXY(float lookX, float lookY) {
	look_X = lookX;
	look_Y = lookY;
}

void Player::setMyLookXY(float mouseX, float mouseY) {
	float num = sqrt(
		(((float)mouseX - draw_X) * ((float)mouseX - draw_X)) +
		(((float)mouseY - draw_Y) * ((float)mouseY - draw_Y))
	);
	look_X = ((float)mouseX - draw_X) / num;
	look_Y = ((float)mouseY - draw_Y) / num;
}

void Player::change_move(int* state) {
	move_State[0] = state[0];
	move_State[1] = state[1];
}

void Player::update(float realX, float realY, float hp) {
	now_HP = hp;
	real_X = realX;
	real_Y = realY;
	draw_X = setXY(realX);
	draw_Y = setXY(realY);
}