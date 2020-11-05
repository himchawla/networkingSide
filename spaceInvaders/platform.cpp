// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2019 Media Design School 
// 
//  File Name   :   platform.cpp
//  Description :   Implemntation for platform class
//  Author      :   Himanshu Chawla
//  Mail        :   Himanshu.Cha8420@mediadesign.mail.nz 
// 
// Implementation


#include "platform.h"


platform::platform(float nx, float ny, float hP) :sprite(nx, ny)
{
	x = nx;
	y = ny;
	sp.setPosition(x, y);
	w = hP;
	h = 32;
	
}

platform::platform():sprite(0,0)
{
	x = 0;
	y = 0;
	w = 32;
	h = 32;
}


void platform::move(int dir, float time)
{
	if (dir == 1)
	{
		y -= 0.2 * time;
	}
	if (dir == -1)
	{
		y += 0.2 * time;
	}

	if (dir == 2)
	{
		x += 0.2 * time;
	}

	if (dir == -2)
	{
		x -= 0.2 * time;
	}

	setLocation(x, y);
}