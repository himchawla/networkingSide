// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2019 Media Design School 
// 
//  File Name   :   platform.h
//  Description :   Include for platform class
//  Author      :   Himanshu Chawla
//  Mail        :   Himanshu.Cha8420@mediadesign.mail.nz 
// 
// Local Include

#pragma once
#include "sprite.h"
class platform : public sprite
{
public:
	platform(float x, float y, float h);
	platform();
	void move(int dir, float time);
};

