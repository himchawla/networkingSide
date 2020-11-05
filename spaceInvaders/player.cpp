// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2019 Media Design School 
// 
//  File Name   :   player.cpp
//  Description :   Implemntation for player class
//  Author      :   Himanshu Chawla
//  Mail        :   Himanshu.Cha8420@mediadesign.mail.nz 
// 
// Implementation


#include "player.h"

int player::getLives()
{
	return (int)lives;
}

void player::addScore()
{
	score++;
}

int player::getScore()
{
	return score;
}

bool player::isJump()
{
	return isJumping;
}

Bullet* player::getBull()
{
	return b1;
}

float player::getSpeed()
{
	return speed;
}

float player::getGrav()
{
	return grav;
}

bool player::isGod()
{
	return god;
}

void player::setGod(bool g)
{
	god = g;
}
sf::Vector2f player::getSpawnPos()
{
	return spawnPos;
}
void player::setGrav(float g)
{
	grav = g;
}

void player::setSpeed(float s)
{
	speed = s;
}

bool player::isAttacking()
{
	return attack;
}

void player::recieveDamage(float dam)
{
	if (health - dam > 0)
	{
		health -= dam;
	}
	else
	{
		kill();
	}
}

void player::Keyboard()
{
	if (key == 'A')
	{
		dir = 1;
		dx = -speed;
		if (sp.getScale().x == -1)
		{
			sp.setScale(1, 1);
			x -= 32;
		}


	}

	
	if (key == 'J')
	{
		dir = 1;
		dx = -speed - speed/3;
		sp.setScale(1, 1);
	}


	if (key == 'D')
	{
		dir = 0;
		dx = speed;
		if (sp.getScale().x == 1)
		{
			sp.setScale(-1, 1);
			x += 32;
		}
	}
	if (key == 'L')
	{
		dir = 0;
		dx = speed + speed/3;
		sp.setScale(-1, 1);
	}

	if (attkey == 'X')
	{
		if (b1 != nullptr)
			if (!b1->alive)
				b1 = new Bullet(x, y, 700.0f, ((dir) ? 1 : -1));
			else 
				std::cout << "";
		else 		
			b1 = new Bullet(x, y, 900.0f, ((dir) ? 1 : -1));


	}
	

	if (key != 'A' && key != 'D' && key != 'J' && key != 'L')
	{
		dx = 0;
	}
	if (verkey == 'V' && !isJumping && grounded)
	{
		dy = -15;

		isJumping = true;
		jumpTimer = 0;
	}
	
	key = '0';
	if(verkey == 'V')
	verkey = '0';
	attkey = '0';


}

void player::setSpawnPos(float x, float y)
{
	spawnPos.x = x;
	spawnPos.y = y;
}

void player::reset()
{
	lives = 4;
}

void player::update(float time)
{
	Keyboard();
	
	

	if (verkey == 'V' && isJumping)
	{
		dy += 0.005f * time;
	}
	if (verkey == 'S')
	{
		if (!grounded)
		{
			dy += grav / 10;
		}
		else dy = 0;
	}

	verkey = '0';
	if(!grounded)
		dy += grav * time;
		x += dx * time;
		
		if ((grounded) && dy >= 0)
		{
			dy = 0;
			isJumping = false;
		}
			

	
		y += dy * time;
		dy = 0;
		setLocation(x, y);

}


player::player(float nx, float ny) :sprite(nx, ny)
{
	option("Player", 0);
	attack = 0;
	damage = 0;
	god = false;
	health = 0;
	isJumping = false;
	key = '0';
	verkey = '0';
	w = 32;
	h = 32;
	x = nx;
	y = ny;
	sp.setPosition(nx,ny);
	cash = 0;
	lives = 4;
	grounded = false;
	jumpTimer = 0;

	grav = 0.0098f;
	speed = 0.3f;

	//obj = lev.GetAllObjects();
}


void player::applyDamage(baseEnemy* en)
{
	en->recieveDamage(damage);
}

bool player::kill()
{
	if (lives > 1 && !god)
	{
		lives--;
		setLocation(spawnPos.x, spawnPos.y);
		x = spawnPos.x;
		y = spawnPos.y; 
		return false;
	}
	else if (!god)
	{
		return true;
	}
	else 
	{
		return false;
	}

}
