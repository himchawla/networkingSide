#pragma once

#include "sprite.h"


class Bullet:public sprite
{
private:
	float startPointX;
	float startPointY;
	float range;
	float speed;
	int dirr;
	float moved;
public:
	Bullet(float x, float y, float r, int d) :sprite(x, y)
	{
		moved = 0.0f;
		startPointX = x;
		startPointY = y;
		range = r;
		dirr = -d;
		alive = true;
		speed = 1.0f;
		sp.setScale(0.1f, 0.1f);
		
	}


	void move(float dt);

};

