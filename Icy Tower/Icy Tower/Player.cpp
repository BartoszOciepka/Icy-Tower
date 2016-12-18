#include "Player.h"


Player::Player()
{
	this->x = 300;
	this->y = 499;
	this->speed = 0.0;
	this->max_speed = 10;
	this->vertical_speed = 0;
	this->jump_height = 150;
	this->jump = false;
}


Player::~Player()
{
}
