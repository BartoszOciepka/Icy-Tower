#include "Map.h"
#include <cmath>
#include "Player.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <vector>


Map::Map()
{
	this->RIGHT_END = 537;				
	this->LEFT_END = 50;				
	this->SPEED_SLOWDOWN = 0.9;		
	this->SPEED_INCREASE = 0.75;
	this->VERTICAL_SLOWDOWN = 0.4;
	this->END_Y = 0;	
	this->DISTANCE_BETWEEN_PLATFORMS = 110;
	this->JUMPING_MULTIPLIER = 0.2;
	this->counter = 1;
	this->moveMap = false;
	this->MINIMAL_VERTICAL_SPEED = 9;
	
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
		this->IceBlockLength.push_back(200);												//
																							//
		PlatformY -= this->DISTANCE_BETWEEN_PLATFORMS;										//
	}																						//
	this->PixelsMoved = PlatformY + this->DISTANCE_BETWEEN_PLATFORMS;
}


Map::~Map()
{
}

void Map::updateSpeed(Player & player, ALLEGRO_KEYBOARD_STATE klawiatura)
{
	if (player.x > this->RIGHT_END)player.x = this->RIGHT_END;			//if champion is outside the tower 
	else if (player.x < this->LEFT_END)player.x = this->LEFT_END;		//teleport to END

	if (player.x == this->RIGHT_END) {							//bouncing from a wall, 
		player.speed = -player.speed - 1;					//needs to be changed to only occur when direction is changed fast after wall hit
	}
	else if (player.x == this->LEFT_END) {
		player.speed = -player.speed + 1;
	}

	else if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && abs(player.speed) < abs(player.max_speed))player.speed += this->SPEED_INCREASE;			//accelerating right

	else if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && abs(player.speed) < abs(player.max_speed))player.speed -= this->SPEED_INCREASE;			//accelerating left


	else player.speed *= this->SPEED_SLOWDOWN;				//slowing down when nothing pressed

	if (abs(player.speed) < 0.05) player.speed = 0;			//stopping when speed low

	if (al_key_down(&klawiatura, ALLEGRO_KEY_UP) && player.vertical_speed == 0) {
		player.vertical_speed = this->MINIMAL_VERTICAL_SPEED + (this->JUMPING_MULTIPLIER * abs(player.speed));																	//IMPORTANT!!! jumping mulipliers
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

	}
}

void Map::MoveCharacter(Player & player, Map map)
{
	player.x += player.speed;
	player.y -= player.vertical_speed;

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
}

void Map::MoveMap(Map & map, Player player) { // moves the platforms down
	std::vector<int>::iterator it = map.YCoordinateIceBlock.begin();

	while(it != YCoordinateIceBlock.end())
	{
		if (*it == 570 || *it == 550)
		{
			int distance = std::distance(map.YCoordinateIceBlock.begin(), it);
			map.YCoordinateIceBlock.erase(it);
			map.IceBlocksXStart.erase(map.IceBlocksXStart.begin() + distance);
			map.IceBlockLength.erase(map.IceBlockLength.begin() + distance);
			it = map.YCoordinateIceBlock.begin();
		}
		else
		{
			*it += 3;
			it++;
		}

		
	}

	PixelsMoved+=3;

	if (map.PixelsMoved % map.DISTANCE_BETWEEN_PLATFORMS == 0)
	{
		this->YCoordinateIceBlock.push_back(0);										//
		this->IceBlocksXStart.push_back((std::rand() % 337) + 50);							//
		this->IceBlockLength.push_back(200);
	}

	//if (player.vertical_speed == 0 && )player.y += 1;					TO DO
}