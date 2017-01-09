#include "Map.h"
#include <cmath>
#include "Player.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <vector>


Map::Map()
{
	this->RIGHT_END = 537;				
	this->LEFT_END = 50;				
	this->SPEED_SLOWDOWN = 0.6;		
	this->SPEED_INCREASE = 0.1;
	this->VERTICAL_SLOWDOWN = 0.05;
	this->DISTANCE_BETWEEN_PLATFORMS = 100;
	this->JUMPING_MULTIPLIER = 0.02;
	this->counter = 1;
	this->moveMap = false;
	this->MINIMAL_VERTICAL_SPEED = 5;
	this->PLATFORM_MOVE_VECTOR = 3;
	this->leftBounceCounter = 0;
	this->rightBounceCounter = 0;
	this->BounceTimer = 60;
	this->speedBoost = 2;
	this->verticalSpeedBoost = 1;
	this->bitmapMove = 600;
	
	this->YCoordinateIceBlock.push_back(570);												//
	this->IceBlocksXStart.push_back(50);													//
	this->IceBlockLength.push_back(600);													//initial platforms
																							//
	int PlatformY = 570 - this->DISTANCE_BETWEEN_PLATFORMS;									//
																							//
	while (PlatformY > 0)																	//
	{																						//
		this->YCoordinateIceBlock.push_back(PlatformY);										//
		this->IceBlocksXStart.push_back((std::rand() % 337) + 50);							//
		this->IceBlockLength.push_back(200);	
		this->AddedToPoints.push_back(false);//
																							//
		PlatformY -= this->DISTANCE_BETWEEN_PLATFORMS;										//
	}																						//
	this->PixelsMoved = PlatformY + this->DISTANCE_BETWEEN_PLATFORMS;
}


Map::~Map()
{
}

bool Map::isOnPlatform(Map & map, Player & player)
{
	for (std::vector<int>::iterator it = map.YCoordinateIceBlock.begin(); it != map.YCoordinateIceBlock.end(); ++it)
	{
		if (*it == (player.y+player.height))return true;
	}
	return false;
}

void Map::updateSpeed(Player & player, ALLEGRO_KEYBOARD_STATE klawiatura, bool & done)
{
	if (player.x > this->RIGHT_END)player.x = this->RIGHT_END;			//if champion is outside the tower 
	else if (player.x < this->LEFT_END)player.x = this->LEFT_END;		//teleport to END

	if (player.x == this->RIGHT_END && player.vertical_speed > 0) {
		player.speed = -player.speed - 0.005;
		this->rightBounceCounter = this->BounceTimer;
	}

	else if (player.x == this->LEFT_END && player.vertical_speed > 0) {
		player.speed = -player.speed + 0.005;
		this->leftBounceCounter = this->BounceTimer;
	}

	else if (player.x == this->RIGHT_END) {							//bouncing from a wall, 
		player.speed = -player.speed - 0.005;					//needs to be changed to only occur when direction is changed fast after wall hit
		player.x -= 1;
	}
	else if (player.x == this->LEFT_END) {
		player.speed = -player.speed + 0.005;
		player.x += 1;
	}

	else if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && abs(player.speed) < abs(player.max_speed) && player.vertical_speed > 0 && this->leftBounceCounter>0)
	{
		player.speed += this->SPEED_INCREASE;			//accelerating right
		player.vertical_speed += this->verticalSpeedBoost*abs(player.speed);
		this->leftBounceCounter = 0;
		ALLEGRO_SAMPLE *sample1 = NULL;
		sample1 = al_load_sample("resources/yuppi.wav");
		if (!sample1)
		{
			this->leftBounceCounter = 0;
		}
		al_play_sample(sample1, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}

	else if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && abs(player.speed) < abs(player.max_speed))player.speed += this->SPEED_INCREASE;			//accelerating right

	else if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && abs(player.speed) < abs(player.max_speed) && player.vertical_speed > 0 && this->rightBounceCounter > 0)
	{
		player.speed -= this->SPEED_INCREASE;			//accelerating left
		player.vertical_speed += this->verticalSpeedBoost*abs(player.speed);
		this->rightBounceCounter = 0;
		ALLEGRO_SAMPLE *sample = NULL;
		sample = al_load_sample("resources/yuppi.wav");
		al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}


	else if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && abs(player.speed) < abs(player.max_speed))player.speed -= this->SPEED_INCREASE;			//accelerating left


	else player.speed *= this->SPEED_SLOWDOWN;				//slowing down when nothing pressed

	if (abs(player.speed) < 0.005) player.speed = 0;			//stopping when speed low

	if (al_key_down(&klawiatura, ALLEGRO_KEY_UP) && player.vertical_speed == 0) {
		player.vertical_speed = this->MINIMAL_VERTICAL_SPEED + (this->JUMPING_MULTIPLIER * abs(player.speed));																	//IMPORTANT!!! jumping mulipliers
		ALLEGRO_SAMPLE *sample = NULL;
		sample = al_load_sample("resources/jump.wav");
		al_play_sample(sample, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
	}

	else if (player.vertical_speed != 0) {					//When in mid-air it checks for platform to land on or slows player
		int min = -1000;										//lowest difference between platform and player
		int position = 0;

		for (std::vector<int>::iterator it = this->YCoordinateIceBlock.begin(); it != this->YCoordinateIceBlock.end(); ++it) //looks for a platform closest to player, but under him
		{																													 
			int tmp = player.y + player.height - *it;		//gets how much lower is the platform under the player, must be >0 if player is over it
			if (tmp > min && tmp <= 0)
			{
				min = tmp;
				position = std::distance(this->YCoordinateIceBlock.begin(), it);											
			}
		}
		if (min != -1000)	//if platform to land wasnt found
		{
			if (abs(min) <= abs(player.vertical_speed) && player.vertical_speed < 0 && (player.x+player.width) > this->IceBlocksXStart[position] && player.x < (this->IceBlocksXStart[position] + this->IceBlockLength[position]))														//then it checks if you will land on the platform, ONLY WHEN FALLING DOWN!!!
			{
				player.y = this->YCoordinateIceBlock[position] - player.height;
				player.vertical_speed = 0;
			}
			else																													//else it slows down player
				player.vertical_speed -= this->VERTICAL_SLOWDOWN;
		}
		else
			player.vertical_speed -= this->VERTICAL_SLOWDOWN;


		//if (abs(min) < 2 && player.x > (this->IceBlocksXStart[position]-player.width) && player.x < (this->IceBlocksXStart[position] + this->IceBlockLength[position]))player.vertical_speed = 0;
		if (player.y > 640)done = true;
		if (this->rightBounceCounter != 0)this->rightBounceCounter--;
		if (this->leftBounceCounter != 0)this->leftBounceCounter--;

	}
}

void Map::MoveCharacter(Player & player, Map & map)
{
	player.x += player.speed;
	player.y -= player.vertical_speed;

	if (player.y < 200)
	{
		map.MoveMap(map, player, 200-player.y);
		player.y = 200;
	}

	if (player.vertical_speed == 0)			//checking if player falls down
	{
		std::vector<int>::iterator it = std::find(map.YCoordinateIceBlock.begin(), map.YCoordinateIceBlock.end(), (player.y+player.height));
		int index = 0;

		if (it != map.YCoordinateIceBlock.end())index = abs(std::distance(map.YCoordinateIceBlock.begin(), it));

		if (player.x < (map.IceBlocksXStart[index] - player.width) ||
			player.x > map.IceBlocksXStart[index] + map.IceBlockLength[index] ||
			it == map.YCoordinateIceBlock.end())
		{
		player.vertical_speed = -0.3;
		}
	}

	std::vector<bool>::iterator it2 = map.AddedToPoints.begin();
	for (std::vector<int>::iterator it = map.YCoordinateIceBlock.begin(); it != map.YCoordinateIceBlock.end(); it++)
	{
		if (*it > player.y && *it2 == false)
		{
			*it2 = true;
			player.points++;
		}
		it2++;
	}
}

void Map::MoveMap(Map & map, Player & player, int platform_move_vector) { // moves the platforms down
	std::vector<int>::iterator it = map.YCoordinateIceBlock.begin();

	if (map.isOnPlatform(map, player))player.y += platform_move_vector;

		this->bitmapMove -= platform_move_vector;

		if (this->bitmapMove < -600)
		{
			this->bitmapMove += 1200;
		}



	while(it != YCoordinateIceBlock.end())
	{
		if (*it > 640)
		{
			int distance = std::distance(map.YCoordinateIceBlock.begin(), it);
			map.YCoordinateIceBlock.erase(it);
			map.IceBlocksXStart.erase(map.IceBlocksXStart.begin() + distance);
			map.IceBlockLength.erase(map.IceBlockLength.begin() + distance);
			map.AddedToPoints.erase(map.AddedToPoints.begin());
			it = map.YCoordinateIceBlock.begin();
		}
		else
		{
			*it += platform_move_vector;
			it++;
		}

	}

	int highestPlatformY = map.YCoordinateIceBlock[map.YCoordinateIceBlock.size() - 1];	//gets highest platform

	highestPlatformY -= map.DISTANCE_BETWEEN_PLATFORMS;		//checks if new platform should be generated, if so highest platforms will be higher than 0

	while (highestPlatformY > 0)
	{
		this->YCoordinateIceBlock.push_back(highestPlatformY);										
		this->IceBlocksXStart.push_back((std::rand() % 337) + 50);							//
		this->IceBlockLength.push_back(200);
		this->AddedToPoints.push_back(false);
		highestPlatformY -= map.DISTANCE_BETWEEN_PLATFORMS;
	}
}