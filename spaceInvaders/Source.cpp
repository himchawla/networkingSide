#include <iostream>
#include <SFML/Graphics.hpp>
#include"player.h"
#include"platform.h"
#include "collision.h"
#include "level.h"
#include<vector>
#include<string>
#include<math.h>
#include <Windows.h>
#include <windowsx.h>
#include <strstream>
#include <sstream>
#include"resource1.h"
#include<fstream>
#include "physics.h"

#include <cassert>
//#include <vld.h>
#include <thread>

//Local Includes
#include "consoletools.h"
#include "network.h"
#include "client.h"
#include "server.h"
#include "InputLineBuffer.h"
#include <functional>

// make sure the winsock lib is included...
#pragma comment(lib,"ws2_32.lib")

bool pSpeedChange = 0, eSpeedChange = 0, god = 0, gravChange, toTransform;
float plaSpeed = 0.0f, enSpeed = 0.0f, gravVal = 0.0f;
float pX = 0, pY = 0;
bool wannaSend = true;



INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)     // Dialog Box
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1:
		{
			if (plaSpeed < 1)
			{
				plaSpeed += 0.05f;
				pSpeedChange = true;
			}
		}
		break;
		case  IDC_BUTTON2:
		{
			if (plaSpeed > 0.5)
			{
				plaSpeed -= 0.5;
				pSpeedChange = true;
			}
		}break;

		case IDC_BUTTON3:
		{
			if (enSpeed < 1)
			{
				enSpeed += 0.05f;
				eSpeedChange = true;
			}
		}
		

		case  IDC_BUTTON4:
		{
			if (enSpeed > 0.5)
			{
				enSpeed -= 0.5;
				eSpeedChange = true;
			}
		}break;

		case IDC_BUTTON5:
		{
			if (gravVal < 0.05)
			{
				gravVal += 0.005f;
				gravChange = true;
			}
		}break;

		case IDC_BUTTON6:
		{
			if (gravVal > 0.005)
			{
				gravVal -= 0.005f;
				gravChange = true;
			}
		}break;
		case IDC_BUTTON7:
		{
			wchar_t s[5];
			GetDlgItemText(hDlg, IDC_EDIT2, s, 5);
			pX = (float)std::stoi(s);
			std::cout << pX;

			wchar_t t[5];
			GetDlgItemText(hDlg, IDC_EDIT3, t, 5);
			pY = (float)std::stoi(t);
			toTransform = true;

		}

		case IDC_BUTTON8:
		{
			std::ofstream dfile;
			dfile.open("Assets/Debug.txt");
			dfile << plaSpeed << " " << enSpeed << " " << gravVal << " " << god;
			dfile.close();
		}break;

		case IDC_BUTTON9:
		{
			std::ifstream dFile;
			bool preGod = god;
			dFile.open("Assets/Debug.txt");
			dFile >> plaSpeed >> enSpeed >> gravVal >> god;
			pSpeedChange = true;
			eSpeedChange = true;
			gravChange = true;
			if (god)
			{
				SetDlgItemTextA(hDlg, IDC_EDIT1, (CHAR*)"Enabled");
			}
			else
			{
				SetDlgItemTextA(hDlg, IDC_EDIT1, (CHAR*)"Disabled");
			}
		}break;

		case IDC_CHECK1:
		{
			if (god)
			{
				god = false;
				SetDlgItemTextA(hDlg, IDC_EDIT1, (CHAR*)"Disabled");
			}
			else
			{
				god = true;
				SetDlgItemTextA(hDlg, IDC_EDIT1, (CHAR*)"Enabled");
			}
		}break;


		case IDOK:
		{
			SendMessage(hDlg, WM_ACTIVATE, 0, 0);
			return true;
		}break;
		case IDCANCEL:
			SendMessage(hDlg, WM_CLOSE, 0, 0);

		}break;

	case WM_ACTIVATE:
	{
		ShowWindow(hDlg, 0);
		return true;
	}
	break;

	case WM_CLOSE:

	{
		ShowWindow(hDlg, 0);
		return true;
	}
	break;
	case WM_DESTROY:
		ShowWindow(hDlg, 0);

		return true;
	}
	return false;
}

// level Manager
enum levell
{
	startScreen,lev1,lev2,lev3,gameOver,win
}lev;

struct packet
{
	sf::Vector2f pos;
	float lives;
	float score;
	sf::Vector2f speed;
	bool grounded;
	std::string uName;

	packet(sf::Vector2f p, float l, float s, sf::Vector2f vel, float groun, std::string u)
	{
		uName = u;
		pos = p;
		lives = l;
		score = s;
		speed = vel;
		grounded = groun;	
	}

	packet()
	{};
}currState(sf::Vector2f(0.0f,0.0f), 1.0f,1.0f,sf::Vector2f(0.0f,0.0f), 1, "ss");


bool hasChanged(player p, packet state)
{
	if (p.dx != state.speed.x || p.dy != state.speed.y)
		return true;
	else return false;
}

void setStufff(player p, packet& state)
{
	state.speed = sf::Vector2f(p.dx, p.dy);
	state.pos = sf::Vector2f(p.x, p.y);
	state.grounded = p.grounded;
	state.lives = p.getLives();

}


std::string toSend(packet p)
{
	std::string temp = "";

	temp.append(p.uName);

	temp += "#";

	temp.append(std::to_string(p.pos.x));

	temp += "#";

	temp.append(std::to_string(p.pos.y));

	temp += "#" + std::to_string(p.lives) + "#" + std::to_string(p.score) + "#" + std::to_string(p.speed.x) + "#" + std::to_string(p.speed.y) + "#" + std::to_string(p.grounded);

	return temp;
}


packet recieved(std::string s)
{
	std::string temp, nTemp;
	packet p;
	temp = &s[0];
	int i = 0;
	while (temp[i] != '#')
	{
		nTemp += temp[i];
		i++;
	}
	p.uName = nTemp;
	nTemp = "";
	temp = &temp[i + 1];
	i = 0;
	while (temp[i] != '#')
	{
		nTemp += temp[i];
		i++;
	}
	p.pos.x = stof(nTemp);
	nTemp = "";
	temp = &temp[i +1];
	i = 0;
	while (temp[i] != '#')
	{
		nTemp += temp[i];
		i++;
	}
	p.pos.y = stof(nTemp);
	nTemp = "";
	temp = &temp[i +1];
	i = 0;
	while (temp[i] != '#')
	{
		nTemp += temp[i];
		i++;
	}
	p.lives = stoi(nTemp);

	nTemp = "";
	temp = &temp[i + 1];
	i = 0;
	while (temp[i] != '#')
	{
		nTemp += temp[i];
		i++;
	}
	p.score = stoi(nTemp);

	nTemp = "";
	temp = &temp[i +1];
	i = 0;
	while (temp[i] != '#')
	{
		nTemp += temp[i];
		i++;
	}
	p.speed.x = stof(nTemp);
	nTemp = "";
	temp = &temp[i +1];
	i = 0;
	while (temp[i] != '#')
	{
		nTemp += temp[i];
		i++;
	}
	p.speed.y = stof(nTemp);
	nTemp = "";
	temp = &temp[i+1];
	i = 0;
	while (temp[i] != '\0')
	{
		nTemp += temp[i];
		i++;
	}
	p.grounded = stoi(nTemp);
	nTemp = "";
	
	return p;



}


int main()
{

	packet p(sf::Vector2f(1.0f, 1.0f), 2.2f, 3.1f, sf::Vector2f(2.0f, 2.0f), 1, "Him");

	std::string test = toSend(p);

	packet p1 = recieved(test);

	char* _pcPacketData = 0; //A local buffer to receive packet data info
	_pcPacketData = new char[MAX_MESSAGE_LENGTH];
	strcpy_s(_pcPacketData, strlen("") + 1, "");

	char _cIPAddress[MAX_ADDRESS_LENGTH]; // An array to hold the IP Address as a string
										  //ZeroMemory(&_cIPAddress, strlen(_cIPAddress));

	unsigned char _ucChoice;
	EEntityType _eNetworkEntityType;
	CInputLineBuffer _InputBuffer(MAX_MESSAGE_LENGTH);
	std::thread _ClientReceiveThread, _ServerReceiveThread;

	//Get the instance of the network
	CNetwork& _rNetwork = CNetwork::GetInstance();
	_rNetwork.StartUp();

	//A pointer to hold a client instance
	CClient* _pClient = nullptr;
	//A pointer to hold a server instance
	CServer* _pServer = nullptr;

	// query, is this to be a client or a server?
	_ucChoice = QueryOption("Do you want to run a client or server (C/S)?", "CS");
	switch (_ucChoice)
	{
	case 'C':
	{
		_eNetworkEntityType = CLIENT;
		break;
	}
	case 'S':
	{
		_eNetworkEntityType = SERVER;
		break;
	}
	default:
	{
		std::cout << "This is not a valid option" << std::endl;
		return 0;
		break;
	}
	}
	if (!_rNetwork.GetInstance().Initialise(_eNetworkEntityType))
	{
		std::cout << "Unable to initialise the Network........Press any key to continue......";
		_getch();
		return 0;
	}

	//Run receive on a separate thread so that it does not block the main client thread.
	if (_eNetworkEntityType == CLIENT) //if network entity is a client
	{

		_pClient = static_cast<CClient*>(_rNetwork.GetInstance().GetNetworkEntity());
		_ClientReceiveThread = std::thread(&CClient::ReceiveData, _pClient, std::ref(_pcPacketData));

	}

	//Run receive of server also on a separate thread 
	else if (_eNetworkEntityType == SERVER) //if network entity is a server
	{

		_pServer = static_cast<CServer*>(_rNetwork.GetInstance().GetNetworkEntity());
		_ServerReceiveThread = std::thread(&CServer::ReceiveData, _pServer, std::ref(_pcPacketData));

	}


	while (_rNetwork.IsOnline() && _eNetworkEntityType == SERVER)
	{
		if (_pServer != nullptr)
		{
			if (!_pServer->GetWorkQueue()->empty())
			{
				_rNetwork.GetInstance().GetNetworkEntity()->GetRemoteIPAddress(_cIPAddress);
				//std::cout << _cIPAddress
				//<< ":" << _rNetwork.GetInstance().GetNetworkEntity()->GetRemotePort() << "> " << _pcPacketData << std::endl;

				//Retrieve off a message from the queue and process it
				std::pair<sockaddr_in, std::string> dataItem;
				_pServer->GetWorkQueue()->pop(dataItem);
				_pServer->ProcessData(dataItem);
			}
		}
	}
	
	if (_eNetworkEntityType == CLIENT)
	{

		sf::RenderWindow window(sf::VideoMode(1920, 1080), "Space Invaders");



		std::ofstream lfile;
		//lfile.open("Assets/level.txt");
		HWND hDlg = CreateDialogParam(NULL, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
		std::ifstream file;
		file.open("Assets/level.txt");
		int lvl;
		lvl = 0;
		lev = startScreen;

		//Texts and messages
		sf::Text msg;
		sf::Text score;
		sf::Text lives;
		sf::Font font;
		font.loadFromFile("Assets/arial.ttf");
		lives.setFont(font);
		lives.setString("");
		score.setFont(font);
		score.setString("");
		msg.setFont(font);
		lives.setPosition(1850, 10);
		score.setPosition(1850, 110);



		bool show = false;

		//Level setup
		level level1(7, 1);
		level level2(2, 1);
		level level3(5, 1);
		level1.placePlats(1);
		level2.placePlats(2);
		level3.placePlats(3);


		bool jedi = false;

		//Base Enemy
		baseEnemy en1(20.0f, 20.0f);
		en1.setTexture("Assets/Enemies/baseEnemy.png");
		std::vector<baseEnemy> enVec;
		enVec.push_back(en1);
		en1.setLocation(100.0f, 500.0f);
		enVec.push_back(en1);
		//moving platform
		platform mover;
		mover.setTexture("Assets/Grass block.png");
		mover.setLocation(360, 200);

		platform ground(200, 900, 600);
		ground.setTexture("Assets/Grass block.png");
		window.setFramerateLimit(60);
		player p1(960, 540);
		p1.setSpawnPos(200, 700);
		p1.setTexture("Assets/Warrior.png");
		baseEnemy en(20.0f, 20.0f);
		sf::Clock sfClock;
		float time = sfClock.restart().asSeconds();
		platform trigger(2500, 800, 64);
		trigger.setTexture("Assets/trigger.png");

		en1.setLocation(520, 835);

		platform death(-200, 960, 64);
		death.setTexture("Assets/Grass block.png");
		death.sp.setColor(sf::Color::Red);
		death.sp.setScale(200, 1);
		//plat.setTexture("Assets/Grass block.png");

		sf::Vector2f center = sf::Vector2f(960.0f, 540.0f);
		sf::Vector2f size = sf::Vector2f(1920.0f, 1080.0f);
		sf::View view1(center, size);
		//float globalTime = 0.0f;

		plaSpeed = p1.getSpeed();
		enSpeed = en1.getSpeed();

	skip:

		/*switch (lvl)
		{
		case 1:
		{
			lev = lev1;
			p1.setSpawnPos(level1.spawnPoints.x, level1.spawnPoints.y);

		}break;
		case 2:
		{
			level1.checks[0].draw(window);

			lev = lev1;
			p1.setSpawnPos(level1.checks[0].x, level1.checks[0].y);
			p1.setLocation(level1.checks[0].x, level1.checks[0].y + 10);
		}break;
		case 3:
		{
			lev = lev2;
			p1.setSpawnPos(level2.spawnPoints.x, level2.spawnPoints.y);
			p1.setLocation(level2.spawnPoints.x, level2.spawnPoints.y + 10);

		}break;
		case 4:
		{
			lev = lev2;
			p1.setSpawnPos(level2.checks[0].x, level2.checks[0].x);
		}break;
		default:
		{
			lev = lev1;
		}
			break;
		}

		file >> lvl;
		*/
		//Game Loop		t = sfClock();




		while (window.isOpen() && _rNetwork.IsOnline())
		{

			_pClient = static_cast<CClient*>(_rNetwork.GetInstance().GetNetworkEntity());

			//Prepare for reading input from the user
			//_InputBuffer.PrintToScreenTop();

			////Get input from the user
			//if (_InputBuffer.Update())
			//{
			//	// we completed a message, lets send it:
			//	int _iMessageSize = static_cast<int>(strlen(_InputBuffer.GetString()));

			//	//Put the message into a packet structure
			//	TPacket _packet;
			//	_packet.Serialize(DATA, const_cast<char*>(_InputBuffer.GetString()));
			//	_rNetwork.GetInstance().GetNetworkEntity()->SendData(_packet.PacketData);
			//	//Clear the Input Buffer
			//	_InputBuffer.ClearString();
			//	//Print To Screen Top
			//	_InputBuffer.PrintToScreenTop();
			//}
			if (_pClient != nullptr)
			{
				//If the message queue is empty 
				if (_pClient->GetWorkQueue()->empty())
				{
					//Don't do anything
				}
				else
				{
					//Retrieve off a message from the queue and process it
					std::string temp;
					_pClient->GetWorkQueue()->pop(temp);
					_pClient->ProcessData(const_cast<char*>(temp.c_str()));
				}
			}






			//Set View
			window.setView(view1);

			sf::Event event;
			while (window.pollEvent(event))
			{
				window.setKeyRepeatEnabled(false);

				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Space))
				{
					p1.verkey = 'V';
				}
			}
			//Debug Transform
			if (toTransform)
			{
				p1.setLocation(pX, pY);
				view1.setCenter(pX, view1.getCenter().y);
				lives.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 500);
				score.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 500);


				toTransform = false;
			}

			//StartScreen
			while (lev == startScreen)
			{
				window.clear();
				while (window.pollEvent(event))
				{

					if (event.type == sf::Event::Closed)
						window.close();
				}
				//Intro
				sf::Text Intro;
				Intro.setFont(font);
				Intro.setPosition(800, 400);
				Intro.setString("Welcome to Coool Game\n Press 'Enter' to start a new game\n Press 'Q' to continue from your last adventure");
				window.draw(Intro);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					//start level 1
					lvl = 1;
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
				{
					//Get level from file
					file >> lvl;
					file.close();
				}

				//Set Level
				switch (lvl)
				{
				case 0:
				{

				}break;
				case 1:
				{
					lev = lev1;
					p1.reset();
					p1.alive = true;
					level2.placePlats(2);
					p1.setSpawnPos(level1.spawnPoints.x, level1.spawnPoints.y);
					p1.setLocation(level1.spawnPoints.x, level1.spawnPoints.y);
					setStufff(p1, currState);
				}break;
				case 2:
				{
					level1.checks[0].draw(window);
					lev = lev1;
					p1.reset();
					p1.alive = true;
					level2.placePlats(2);
					p1.setSpawnPos(level1.checks[0].x, level1.checks[0].y);
					p1.setLocation(level1.checks[0].x, level1.checks[0].y - 10);
				}break;
				case 3:
				{
					lev = lev2;
					p1.reset();
					p1.alive = true;
					level2.placePlats(2);
					p1.setSpawnPos(level2.spawnPoints.x, level2.spawnPoints.y);
					p1.setLocation(level2.spawnPoints.x, level2.spawnPoints.y - 10);

				}break;
				case 4:
				{
					lev = lev2;
					p1.reset();
					p1.alive = true;
					level2.placePlats(2);
					p1.setSpawnPos(level2.checks[0].x, level2.checks[0].y);
					p1.setLocation(level2.checks[0].x, level2.checks[0].y);
				}break;
				case 5:
				{
					lev = lev3;
					p1.reset();
					p1.alive = true;
					level2.placePlats(2);
					p1.setSpawnPos(level3.spawnPoints.x, level3.spawnPoints.y);
					p1.setLocation(level3.spawnPoints.x, level3.spawnPoints.y);
				}
				case 6:
				{
					lev = lev3;
					p1.reset();
					p1.alive = true;
					level2.placePlats(2);
					p1.setSpawnPos(level3.checks[0].x, level3.checks[0].y);
				}
				default:
				{
					lvl = 1;
				}
				break;
				}

				window.display();
			}



			//Debug Gravity Change
			if (gravChange)
			{
				p1.setGrav(gravVal);
				gravChange = false;
			}

			//Debug Player Speed
			if (pSpeedChange)
			{
				p1.setSpeed(plaSpeed);
				pSpeedChange = false;
			}


			//Debug Enemy Speed
			if (eSpeedChange)
			{
				en1.setSpeed(enSpeed);
				eSpeedChange = false;
			}

			//Debug Player God
			if (god)
			{
				p1.setGod(true);
			}
			else
			{
				p1.setGod(false);
			}


			float deltaTime = (float)sfClock.restart().asMilliseconds();

			//globalTime += deltaTime;
			window.clear();
			//p1.setLocation(50, 50);


			//Player Movement
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					p1.key = 'J';
				else
					p1.key = 'A';
				if (p1.sp.getGlobalBounds().left < view1.getCenter().x - 100)
				{
					view1.setCenter(view1.getCenter().x + p1.dx * deltaTime, view1.getCenter().y);
					lives.setPosition(lives.getPosition().x + p1.dx * deltaTime, lives.getPosition().y);
					score.setPosition(lives.getPosition().x + p1.dx * deltaTime, score.getPosition().y);


				}


			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{

				if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					p1.key = 'L';
				else
					p1.key = 'D';
				if (p1.sp.getGlobalBounds().left > view1.getCenter().x + 100)
				{
					view1.setCenter(view1.getCenter().x + p1.dx * deltaTime, view1.getCenter().y);
					lives.setPosition(lives.getPosition().x + p1.dx * deltaTime, lives.getPosition().y);
					score.setPosition(lives.getPosition().x + p1.dx * deltaTime, score.getPosition().y);

				}


			}


			if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
			{
				p1.attkey = 'X';
			}

			//Debug Menu
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				ShowWindow(hDlg, 1);
			}

			
			p1.update(deltaTime);
			p1.draw(window);

			if (p1.getBull() != nullptr)
			{
				p1.getBull()->setTexture("Assets/bullet.png");

				p1.getBull()->move(deltaTime);
				p1.getBull()->draw(window);
			}

			//Damage Player
			if (p1.y > 1000)
			{
				if (!god)
				{
					if (p1.kill())
					{
						lev = gameOver;
					}
				}
				else
				{
					p1.setLocation(p1.getSpawnPos().x, p1.getSpawnPos().y);
				}
				view1.setCenter(p1.x, 540);
				lives.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 500);
				lives.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 400);

			}

			//Level 1 Hajime
			if (lev == lev1)
			{

				// Message
				msg.setString("Check if something changed in the back");
				msg.setPosition(2300, 400);
				if (show)
					window.draw(msg);

				//Moving platform test
				mover.draw(window);
				if (jedi && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					mover.move(1, deltaTime);
				}
				if (jedi && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					mover.move(-1, deltaTime);
				}

				lives.setString(std::to_string(p1.getLives()));
				window.draw(lives);
				//en1.setLocation(40, 950);
				score.setString(std::to_string(p1.getScore()));
				window.draw(score);

				enVec[0].move(1000, 1, p1, deltaTime);
				enVec[0].draw(window);

				enVec[1].move(400, 1, p1, deltaTime);
				enVec[1].draw(window);

				level1.portal.draw(window);
				// Level Promote
				if (Collision::BoundingBoxTest(level1.portal.sp, p1.sp))
				{
					lfile.open("Assets/level.txt");
					p1.setSpawnPos(level2.spawnPoints.x, level2.spawnPoints.y);
					lev = lev2;
					show = false;
					lfile << 3;
					lfile.close();
				}
				for (int i = 0; i < 5; i++)
				{
					level1.plats[i].draw(window);
				}

				for (auto& i : enVec)
				{
					if (Collision::BoundingBoxTest(i.sp, p1.sp))
					{
						if (p1.kill())

							lev = gameOver;
						view1.setCenter(p1.x, 540);
						lives.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 500);
						score.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 400);
					}
					if (p1.getBull() != nullptr && i.alive)
					{
						if (Collision::BoundingBoxTest(i.sp, p1.getBull()->sp))
						{
							p1.getBull()->destroy();
							i.alive = false;
							p1.addScore();
						}
					}
				}
				level1.checks[0].draw(window);
				if (Collision::BoundingBoxTest(level1.checks[0].sp, p1.sp))
				{
					lfile.open("Assets/level.txt");
					lfile << 2;
					lfile.close();
					p1.setSpawnPos(level1.checks[0].sp.getPosition().x, level1.checks[0].sp.getPosition().y);
					jedi = true;
					show = true;
					mover.sp.setColor(sf::Color::Green);
				}
				else
				{
					jedi = false;
					mover.sp.setColor(sf::Color::White);
				}
				int num;
				if (show)
					num = 7;
				else num = 5;

				//Platform Collisiions
				for (int i = 0; i < num; i++)
				{
					if (Collision::BoundingBoxTest(level1.plats[i].sp, p1.sp))
					{
						if (p1.y < level1.plats[i].y - 60)
						{
							p1.grounded = true;
							break;
						}

						else
						{
							if (p1.dx < 0 && p1.x > level1.plats[i].x)
							{
								p1.x += 10;
								p1.sp.setPosition(p1.x, p1.y);
							}
							if (p1.dx > 0 && p1.x - 32 < level1.plats[i].x)
							{
								p1.x -= 10;
								p1.sp.setPosition(p1.x, p1.y);
							}
						}
					}
					else p1.grounded = false;
				}
				{
					//Platforms invisible unless you go through checkpoint
					level1.plats[4].draw(window);
					Collision::BoundingBoxTest(level1.plats[4].sp, p1.sp);
					//level1.plats[i].draw(window);
					if (show)
					{
						level1.plats[5].draw(window);

					}
				}
			}

			//level 2 code
			if (lev == lev2)
			{

				// Message
				msg.setString("See that Platform just turned green\n use Up and Down arrow to move it");
				msg.setPosition(1000, 400);
				if (jedi)
					window.draw(msg);
				lives.setString(std::to_string(p1.getLives()));
				window.draw(lives);

				score.setString(std::to_string(p1.getScore()));
				window.draw(score);
				//draw level platforms
				level2.plats[0].draw(window);
				level2.plats[1].draw(window);
				level2.trigger.draw(window);
				level2.portal.draw(window);
				//Collisions  for platforms
				for (int i = 0; i < 2; i++)
				{
					if (Collision::BoundingBoxTest(level2.plats[i].sp, p1.sp))
					{
						if (p1.y < level2.plats[i].y - 60)
						{
							p1.grounded = true;
							break;
						}

						else
						{
							if (p1.dx < 0 && p1.x > level2.plats[i].x)
							{
								p1.x += 10;
								p1.sp.setPosition(p1.x, p1.y);
							}
							if (p1.dx > 0 && p1.x - 32 < level2.plats[i].x)
							{
								p1.x -= 10;
								p1.sp.setPosition(p1.x, p1.y);
							}
						}
					}
					else p1.grounded = false;
				}

				// Next level code
				if (Collision::BoundingBoxTest(level2.portal.sp, p1.sp))
				{
					lfile.open("Assets/level.txt");
					lfile << 5;
					lfile.close();
					p1.setSpawnPos(level3.spawnPoints.x, level3.spawnPoints.y);
					p1.setLocation(level3.spawnPoints.x, level3.spawnPoints.y);
					view1.setCenter(level3.spawnPoints.x, 540);
					lev = lev3;

				}

				// Trigger moving platform

				if (Collision::BoundingBoxTest(level2.trigger.sp, p1.sp))
				{
					level2.plats[1].sp.setColor(sf::Color::Green);
					jedi = true;
				}
				else
				{
					level2.plats[1].sp.setColor(sf::Color::White);
					jedi = false;
				}
				//Move platforms
				if (jedi && sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				{
					level2.plats[1].move(1, deltaTime);
				}

				if (jedi && sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				{
					level2.plats[1].move(-1, deltaTime);
				}


				//if (Collision::BoundingBoxTest(p1.sp, level2.plats[1].sp))
				//{
				//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				//	{
				//		//level2.plats[1].setLocation(level2.plats[1].x - 100, level2.plats[1].y);
				//		level2.plats[1].sp.setOrigin(35, 0.5);
				//		if (level2.plats[1].sp.getRotation() < 45 || (level2.plats[1].sp.getRotation()<360 && level2.plats[1].sp.getRotation()>275))
				//		level2.plats[1].sp.rotate(0.2 * deltaTime);
				//		p1.y = p1.y + (tan(level2.plats[1].sp.getRotation()) / (p1.x - level2.plats[1].x));
				//		p1.setLocation(p1.x, p1.y);
				//		//level2.plats[1].setLocation(level2.plats[1].x + 100, level2.plats[1].y);
				//	}

				//	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				//	{
				//		
				//		//level2.plats[1].setLocation(level2.plats[1].x + 100, level2.plats[1].y);
				//		level2.plats[1].sp.setOrigin(35, 0.5);
				//		if (level2.plats[1].sp.getRotation() < 275 || level2.plats[1].sp.getRotation() > 90)
				//		level2.plats[1].sp.rotate(-0.2 * deltaTime);
				//		p1.y = p1.y + ((level2.plats[1].x - p1.x)/ tan(level2.plats[1].sp.getRotation()));
				//		p1.setLocation(p1.x, p1.y);
				//		//level2.plats[1].setLocation(level2.plats[1].x - 100, level2.plats[1].y);
				//	}
				//}
			}
			// Level 3
			if (lev == lev3)
			{
				msg.setString("A Lever, I wonder what it can do.\nTry using Left and Right arrow keys");
				msg.setPosition(1000, 800);
				if (jedi)
					window.draw(msg);
				lives.setString(std::to_string(p1.getLives()));
				score.setString(std::to_string(p1.getScore()));
				lives.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 500);
				window.draw(lives);


				score.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 400);
				window.draw(score);
				//Draw Lever
				level3.trigger.draw(window);

				//Draw Platforms
				for (int i = 0; i < 2; i++)
				{
					level3.plats[i].draw(window);
				}

				level3.portal.setTexture("Assets/portal.png");
				//Draw portal
				level3.portal.draw(window);

				// Return platform to origin
				if (!Collision::BoundingBoxTest(p1.sp, level3.plats[1].sp))
				{
					if (level3.plats[1].x > 650)
					{
						level3.plats[1].move(-2, deltaTime);
						level3.trigger.move(-2, deltaTime);
					}
					if (level3.plats[1].x < 600)
					{
						level3.plats[1].move(2, deltaTime);
						level3.trigger.move(2, deltaTime);
					}
				}



				//Game End
				if (Collision::BoundingBoxTest(p1.sp, level3.portal.sp))
				{
					lev = win;
				}

				//Move Platform
				if (Collision::BoundingBoxTest(level3.trigger.sp, p1.sp))
				{
					jedi = true;
				}
				else
				{
					jedi = false;
					level3.trigger.setTexture("Assets/leverStraight.png");
				}

				if (jedi && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				{
					level3.trigger.setTexture("Assets/leverLeft.png");
					level3.plats[1].move(-2, deltaTime);
					level3.trigger.move(-2, deltaTime);
					p1.x -= 0.2f * deltaTime;
					p1.setLocation(p1.x, p1.y);
					view1.setCenter(view1.getCenter().x - 0.2f * deltaTime, view1.getCenter().y);
					lives.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 500);
					score.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 400);
				}


				else if (jedi && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				{
					level3.trigger.setTexture("Assets/leverRight.png");
					level3.plats[1].move(2, deltaTime);
					level3.trigger.move(2, deltaTime);
					p1.x += 0.2f * deltaTime;
					p1.setLocation(p1.x, p1.y);
					view1.setCenter(view1.getCenter().x + 0.2f * deltaTime, view1.getCenter().y);
					lives.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 500);
					score.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 400);
				}

				else
					level3.trigger.setTexture("Assets/leverStraight.png");
				//Platform Collissions
				for (int i = 0; i < 2; i++)
				{
					if (Collision::BoundingBoxTest(level3.plats[i].sp, p1.sp))
					{
						if (p1.y < level3.plats[i].y - 60)
						{
							p1.grounded = true;
							break;
						}

						else
						{
							if (p1.dx < 0 && p1.x > level3.plats[i].x)
							{
								p1.x += 10;
								p1.sp.setPosition(p1.x, p1.y);
							}
							if (p1.dx > 0 && p1.x - 32 < level3.plats[i].x)
							{
								p1.x -= 10;
								p1.sp.setPosition(p1.x, p1.y);
							}
						}
					}
					else p1.grounded = false;
				}
			}
			//GameOver Code
			if (lev == gameOver)
			{
				lfile.open("Assets/level.txt");
				lfile << 1;
				lfile.close();
				p1.alive = false;
				view1.setCenter(960, 540);
				sf::Text gmOver;
				gmOver.setFont(font);
				gmOver.setPosition(800, 400);
				gmOver.setString("Game Over\nTry again\n Press Enter to retry\n Press T to go to titleScreen");
				gmOver.setScale(3, 3);
				window.draw(gmOver);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					lev = lev1;
					p1.reset();
					p1.setLocation(level1.spawnPoints.x, level1.spawnPoints.y);
					view1.setCenter(p1.x, 540);
					lives.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 500);
					score.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 400);
					p1.alive = true;
					level2.placePlats(2);
					level1.placePlats(1);
					level3.placePlats(3);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
				{
					lvl = 0;
					lev = startScreen;
				}
			}

			//Win Code
			if (lev == win)
			{
				lfile.open("Assets/level.txt");
				lfile << 1;
				lfile.close();
				p1.alive = false;
				view1.setCenter(960, 540);
				sf::Text gmOver;
				gmOver.setFont(font);
				gmOver.setString("Congratulations You won\n Press Enter to play again\nPress T to Return to main menu");
				gmOver.setScale(3, 3);
				window.draw(gmOver);
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
				{
					lev = lev1;
					p1.reset();
					p1.setLocation(level1.spawnPoints.x, level1.spawnPoints.y);
					view1.setCenter(p1.x, 540);
					lives.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 500);
					score.setPosition(view1.getCenter().x + 900, view1.getCenter().y - 400);
					p1.alive = true;
					level2.placePlats(2);
					level1.placePlats(1);
					level3.placePlats(3);
				}
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
				{
					lvl = 0;
					lev = startScreen;
				}

			}

			if (hasChanged(p1, currState))
			{
				std::cout << "Yess" << std::endl;
			}
			else
			{
				std::cout << "Noooooooo" << std::endl;
			}

			setStufff(p1, currState);
			//	//Put the message into a packet structure
			TPacket _packet;
			std::string msg = toSend(currState);

			_packet.Serialize(DATA, &msg[0]);
			_rNetwork.GetInstance().GetNetworkEntity()->SendData(_packet.PacketData);
			window.display();

		}

		_ClientReceiveThread.join();
		_ServerReceiveThread.join();

		//Shut Down the Network
		_rNetwork.ShutDown();
		_rNetwork.DestroyInstance();

		delete[] _pcPacketData;
		return 0;
	}
}

