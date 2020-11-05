#include "Bullet.h"

void Bullet::move(float dt)
{
	dx = speed * dirr;
	x += dx * dt;
	moved += speed * dt;
	if (moved > range)
	{
		destroy();
	}
	sp.setPosition(x, y);

}

