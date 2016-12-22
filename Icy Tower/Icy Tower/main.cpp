#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <windows.h>
#include <string>
#include <cmath>
#include "Player.h"
#include "Map.h"


//*********************************************************** CONTROL PANEL ********************************************************************

const float FPS = 60;					//FPS number
bool done = false;						//true if program should end


//**********************************************************************************************************************************************

int main()
{ 
	srand(time(NULL));
	Player player;
	Map map;

	al_init();
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_KEYBOARD_STATE klawiatura;
	ALLEGRO_DISPLAY *okno = al_create_display(640, 600);
	ALLEGRO_FONT *font = al_load_ttf_font("raidercrusader.ttf", 72, 0);
	al_set_window_title(okno, "Icy Tower");
	ALLEGRO_BITMAP *obrazek = al_load_bitmap("icytower-bck.png");
	ALLEGRO_BITMAP * player_bitmap = al_load_bitmap("champ.png");
	ALLEGRO_BITMAP * platform = al_load_bitmap("platform.png");
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(okno));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	bool move = false;																//Need to go away, works but is not intuitive

	al_start_timer(timer);
	
	while (!done)
	{
		map.counter++;
		al_get_keyboard_state(&klawiatura);
		al_clear_to_color(al_map_rgb(0, 255, 0));
		al_draw_bitmap(obrazek, 0, 0, 0);
		al_draw_bitmap(player_bitmap, player.x, player.y, 0);
		for (std::vector<int>::iterator it = map.YCoordinateIceBlock.begin(), it2 = map.IceBlocksXStart.begin(); it != map.YCoordinateIceBlock.end(), it2 != map.IceBlocksXStart.end(); ++it, ++it2)
		{
			if (*it != 570)
			{
				al_draw_bitmap(platform, *it2, *it, 0);
			}
		}
		std::string x = std::to_string(player.speed);
		const char *y = x.c_str();
		al_draw_text(font, al_map_rgb(255, 255, 255), 640 / 2, (480 / 4), ALLEGRO_ALIGN_CENTRE, y);
		x = std::to_string(player.vertical_speed);
		const char*z = x.c_str();
		al_draw_text(font, al_map_rgb(255, 255, 255), 640 / 2, (200 / 4), ALLEGRO_ALIGN_CENTRE, z);
		al_flip_display();

		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)done = true;
		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			map.MoveCharacter(player, map);

			map.updateSpeed(player, klawiatura);

			if (player.y < 400 || move )
			{
				move++;
				if (map.counter % 5 == 0)
					map.MoveMap(map, player);
			}
			
		}
	}

	al_destroy_display(okno);
	al_destroy_bitmap(obrazek);

	return 0;
}
