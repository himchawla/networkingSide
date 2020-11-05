// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2019 Media Design School 
// 
//  File Name   :   baseEnemy.cpp
//  Description :   Implemntation for baseEnemy class
//  Author      :   Himanshu Chawla
//  Mail        :   Himanshu.Cha8420@mediadesign.mail.nz 
// 
// Implementation

#include "baseEnemy.h"

baseEnemy::baseEnemy(float x, float y):sprite(x,y)
{
	health = 1.0f;
	damage = 1.0f;
	setTexture("Assets/Enemies/baseEnemy.png");
	speed = 0.2f;
	dir = 1;
	startpoint = (int)x;
}

void baseEnemy::applyDamage()
{
	
}

void baseEnemy::recieveDamage(float dam)
{
	if (health - dam >= 0)
	{
		health -= damage;
	}
	else
	{
		kill();
	}
}


void baseEnemy::kill()
{
	destroy();
}

float baseEnemy::getDamage()
{
	return damage;
}

void baseEnemy::move(int petrolDist, int follDist, sprite player, float time)
{
	dx = dir * speed;
	x += dx * time;
	if (x <= startpoint - petrolDist || x >= startpoint + petrolDist)
		dir *= -1;
	sp.setPosition(x, y);

	
	//else if (x < startpoint - petrolDist || x > startpoint + petrolDist)
	//{
	//	x = (int)startpoint;
	//	sp.setPosition(x, y);
	//}
}

float baseEnemy::getSpeed()
{
	return speed;
}

void baseEnemy::setSpeed(float s)
{
	speed = s;
}


void baseEnemy::setLocation(float x, float y)
{
	startpoint = (int)x;
	sprite::x = x;
	sprite::y = y;
	sp.setPosition(x, y);
}
