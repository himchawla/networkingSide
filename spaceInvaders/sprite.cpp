// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2019 Media Design School 
// 
//  File Name   :	sprite.cpp
//  Description :	Implementation for sprite class
//  Author      :   Himanshu Chawla
//  Mail        :   Himanshu.Cha8420@mediadesign.mail.nz 
// 
 // Implementation 



#include "sprite.h"
#include <stdexcept>

void sprite::setLocation(float x, float y)
{
	sprite::x = x;
	sprite::y = y;
	sp.setPosition(x, y);
}


void sprite::setTexture(std::string t)
{
	tx.loadFromFile(t);
}

sprite::sprite(float x, float y)
{
	alive = true;
	setLocation(x, y);
	
}




void sprite::draw(sf::RenderWindow& win)
{	
	if (alive)
	{
		sp.setTexture(tx);
		win.draw(sp);
	}
}

sf::FloatRect sprite::getRect()
{
	return sf::FloatRect(x,y,w,h);
}

void sprite::option(std::string NAME, float SPEED = 0)
{
	
		Name = NAME;
		
		dx = SPEED;
	
}

void sprite::revive()
{
	alive = true;
}

void sprite::destroy()
{
	alive = false;
}

sprite::~sprite()
{

}

void sprite::update()
{

}