#include <vector>
#pragma once

class Player;
struct ALLEGRO_KEYBOARD_STATE;

class Map
{
public:
	
	int RIGHT_END;				//Right boundaries
	int LEFT_END;				//Left boundaries
	float SPEED_SLOWDOWN;		//How much the speed slows when no key pressed
	float SPEED_INCREASE;		//How much the speed increases when no key pressed
	float VERTICAL_SLOWDOWN;	//Makes the character fall down
	std::vector <int> YCoordinateIceBlock;	//vector to store Y-coordinates on which character can stand
	std::vector <int> IceBlocksXStart;		//vector to store the X-coordinates on which character can stand
	std::vector <int> IceBlockLength;		//length of the platforms, currently stuck at 100 but willget changed
	std::vector <bool> AddedToPoints;
	int DISTANCE_BETWEEN_PLATFORMS;		//just what it says, how far away are the platforms
	float JUMPING_MULTIPLIER;			//says how much vertical_speed you will gain, the function for it is 5 + (JUMPING_MULTIPLIER * player.speed)
	int counter;
	bool moveMap;
	int PixelsMoved;
	int MINIMAL_VERTICAL_SPEED;
	int PLATFORM_MOVE_VECTOR;			//tells how much platforms move down, used in MovePlatform method
	int leftBounceCounter;
	int rightBounceCounter;
	int BounceTimer;
	int speedBoost;
	float verticalSpeedBoost;
	int bitmapMove;

	Map();
	

	void updateSpeed(Player & player, ALLEGRO_KEYBOARD_STATE klawiatura, bool & done);
	void MoveCharacter(Player & player, Map & map);
	void MoveMap(Map & map, Player & player, int platform_move_vector);
	bool isOnPlatform(Map & map, Player & player);
	~Map();
};


