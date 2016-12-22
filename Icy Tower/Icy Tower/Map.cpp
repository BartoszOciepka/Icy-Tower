#include "Map.h"
#include <cmath>
#include "Player.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <algorithm>
#include <iterator>


Map::Map()
{
	this->RIGHT_END = 537;				
	this->LEFT_END = 50;				
	this->SPEED_SLOWDOWN = 0.9;		
	this->SPEED_INCREASE = 0.75;
	this->VERTICAL_SLOWDOWN = 0.4;
	this->END_Y = 0;						
	this->YCoordinateIceBlock.push_back(500);	
	this->IceBlocksXStart.push_back(0);
	this->IceBlocksXEnd.push_back(300);
}


Map::~Map()
{
}

//						CHANGE ALL 70 TO CONST VALUE !!!

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

	else if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && abs(player.speed) < abs(player.max_speed))player.speed += this->SPEED_INCREASE;

	else if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && abs(player.speed) < abs(player.max_speed))player.speed -= this->SPEED_INCREASE;


	else player.speed *= this->SPEED_SLOWDOWN;

	if (abs(player.speed) < 0.05) player.speed = 0;			//stopping when speed low

	if (al_key_down(&klawiatura, ALLEGRO_KEY_UP) && player.vertical_speed == 0) {
		player.vertical_speed = 8;
	}

	else if (player.vertical_speed != 0) {					//When in mid-air it checks for platform to land on or slows player
		int min = 1000;										//lowest difference between platform and player
		int position = 0;

		for (std::vector<int>::iterator it = this->YCoordinateIceBlock.begin(); it != this->YCoordinateIceBlock.end(); ++it) //looks for a platform closest to player
		{																													 
			int tmp = abs(*it - player.y - 70);
			if (tmp < min)
			{
				min = tmp;
				position = std::distance(this->YCoordinateIceBlock.begin(), it);											
			}
		}

		if (min <= abs(player.vertical_speed) && player.vertical_speed < 0)														//then it checks if you will land on the platform, ONLY WHEN FALLING DOWN!!!
		{
			player.y = this->YCoordinateIceBlock[position] - 70;								
			player.vertical_speed = 0;
		}
		else																													//else it slows down player
			player.vertical_speed -= this->VERTICAL_SLOWDOWN;
	}
}

void Map::MoveCharacter(Player & player)
{
	player.x += player.speed;
	player.y -= player.vertical_speed;
}