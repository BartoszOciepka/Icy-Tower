#pragma once
class Player
{
public:
	Player();
	~Player();
	int x;
	int y;
	float speed;
	float max_speed;
	float vertical_speed;
	float jump_height;
	bool jump;						//true if character is jumping
	int height;
	int width;
};

