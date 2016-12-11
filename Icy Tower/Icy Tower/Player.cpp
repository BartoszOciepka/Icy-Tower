#include "Player.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>


Player::Player()
{
	this->x = 300;
	this->y = 499;
	this->speed = 0;
	this->vertical_speed = 0;
	this->jump_height = 150;

	//this->player_bitmap = al_load_bitmap("champ.png");
}


Player::~Player()
{
}
