#include "Player.h"


Player::Player()
{
	this->x = 300;
	this->y = 500;
	this->speed = 0.0;
	this->max_speed = 6;
	this->vertical_speed = 0;
	this->jump = false;
	this->height = 70;
	this->width = 50;
	this->points = -1;
}


Player::~Player()
{
}
