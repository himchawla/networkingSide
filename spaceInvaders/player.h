// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2019 Media Design School 
// 
//  File Name   :   player.h
//  Description :   Include for player class
//  Author      :   Himanshu Chawla
//  Mail        :   Himanshu.Cha8420@mediadesign.mail.nz 
// 
// Local Include

#pragma once
#include "sprite.h"
#include"Bullet.h"
#include "baseEnemy.h"
class player :
	public sprite
{
private:
	float health;
	bool attack;
	float lives;
	float damage;
	sf::Vector2f spawnPos;
	int cash;
	bool isJumping;
	int jumpTimer;
	float speed;
	float grav;
	bool god;
	Bullet *b1;
	int score;

public:
	int getLives();

	void setLives(int l);
	int getScore();
	void addScore();
	void setScore(int s);
	bool isJump();
	Bullet* getBull();
	bool grounded;
	float getSpeed();
	float getGrav();
	bool isGod();
	void setGod(bool g);
	void reset();
	sf::Vector2f getSpawnPos();
	void setGrav(float gr);
	void setSpeed(float s);
	bool isAttacking();
	player(float x, float y);
	char key;
	char verkey;
	char attkey;
	void Key();
	void setSpawnPos(float x, float y);
	void Keyboard();
	void update(float time);

	void ups(float time);
	
	void recieveDamage(float dam);
	void applyDamage(baseEnemy* en);
	bool kill();

};

