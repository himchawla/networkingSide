// 
//  Bachelor of Software Engineering 
//  Media Design School 
//  Auckland 
//  New Zealand 
// 
//  (c) 2019 Media Design School 
// 
//  File Name   :   level.cpp
//  Description :   Implemntation for level class
//  Author      :   Himanshu Chawla
//  Mail        :   Himanshu.Cha8420@mediadesign.mail.nz 
// 
// Implementation


#include "level.h"


level::level(int platNum, int checkNum)
{
	checks = new platform[checkNum];
	plats = new platform[platNum];
	chec = checkNum;
	plat = platNum;
	spawnPoints = sf::Vector2f(0, 0);
}

void level::placePlats(int ch)
{
	if (ch == 1)
	{
		spawnPoints = sf::Vector2f(400.0f, 800.0f);
		for (int i = 0; i < 4; i++)
		{
			plats[i].setTexture("Assets/Grass block.png");
			plats[i].sp.setScale(10, 1);
			plats[i].x = 700 * i + 150;
			plats[i].y = 900 - i * 200;
			plats[i].sp.setPosition(plats[i].x, plats[i].y);
		}
		plats[4].setLocation(700 * 3 + 450, 900 - 3 * 200);
		plats[4].setTexture("Assets/Grass block.png");
		plats[4].sp.setScale(10, 1);
		portal.setTexture("Assets/portal.png");
		portal.setLocation(720, 200);
		for (int i = 5; i < plat; i++)
		{
			plats[i].setTexture("Assets/Grass block.png");
			plats[i].sp.setScale(10, 1);
			plats[i].x = 1700 - i * 150;
			plats[i].y = 900 - 3 * 200;
			plats[i].sp.setPosition(plats[i].x, plats[i].y);

		}
		plats[0].sp.setScale(20, 1);

		checks[0].setLocation(plats[3].sp.getPosition().x + 320, plats[3].sp.getPosition().y - 64);
		checks[0].setTexture("Assets/trigger.png");
		spawnPoints.x = 950;
		spawnPoints.y = 500;



	}

	if (ch == 2)
	{
		trigger.setTexture("Assets/trigger.png");
		for (int i = 0; i < plat; i++)
		{
			plats[i].setTexture("Assets/Grass block.png");
			plats[i].sp.setScale(10, 1);
		}

		spawnPoints.x = 750;
		spawnPoints.y = 600;

		plats[0].sp.setScale(20, 1);

		plats[0].setLocation(10, 900);

		plats[1].sp.setScale(10, 1);


		plats[1].setLocation(1500, 20);
		plats[1].sp.setOrigin(35, 0.5);


		trigger.setLocation(1250, 837);
		portal.setTexture("Assets/portal.png");
		portal.setLocation(1900, 500);
		plats[1].sp.setOrigin(0.5, 0.5);

	}

	if (ch == 3)
	{
		portal.setLocation(2200, 400);
		trigger.setTexture("Assets/leverStraight.png");
		for (int i = 0; i < plat; i++)
		{
			plats[i].setTexture("Assets/Grass block.png");
			plats[i].sp.setScale(10, 1);
		}

		plats[0].sp.setScale(10, 1);
		plats[0].setLocation(100, 700);
		plats[1].sp.setScale(15, 1);
		plats[1].setLocation(650, 600);
		trigger.setLocation(1000, 500);
		trigger.sp.setScale(4, 4);
		spawnPoints.x = 550;
		spawnPoints.y = 250;
	}
}