// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2019 Media Design School 
// 
//  File Name   :   level.h
//  Description :   Include for level class
//  Author      :   Himanshu Chawla
//  Mail        :   Himanshu.Cha8420@mediadesign.mail.nz 
// 
// Local Include

#pragma once
#include "platform.h"

class level
{
private:
	
public:
	int plat;
	int chec;
	sf::Vector2f spawnPoints;
	level(int platNum, int checkNum);
	void placePlats(int);
	platform *plats;
	platform portal;
	platform trigger;
	platform* checks;

};

