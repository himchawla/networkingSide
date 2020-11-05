// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2019 Media Design School 
// 
//  File Name   :   baseEnemy.h
//  Description :   Include for baseEnemy class
//  Author      :   Himanshu Chawla
//  Mail        :   Himanshu.Cha8420@mediadesign.mail.nz 
// 
// Local Include


#pragma once



#include"sprite.h"

class baseEnemy : public sprite
{
private:

protected:
	float health;
	float damage;
	int dir;
	int startpoint;
	float speed;
public:

	//Bullet* b = new Bullet(0.0f, 0.0f, 100.0f, 1);
	baseEnemy(float x, float y);
	void applyDamage();
	void recieveDamage(float damage);
	void kill();
	float getDamage();
	void move(int petrolDist, int follDist, sprite player,float time);
	float getSpeed();
	void setSpeed(float s);
	void setLocation(float x, float y);

};

