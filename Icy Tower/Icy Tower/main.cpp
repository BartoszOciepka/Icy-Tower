#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <windows.h>
#include <string>
#include <cmath>
#include <algorithm>
#include <iterator>
#include "Player.h"
#include "Map.h"


//*********************************************************** CONTROL PANEL ********************************************************************

const float FPS = 200;					//FPS number
bool done = true;						//true if program should end
bool done1 = true;
bool menu = false;

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
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10);
	ALLEGRO_KEYBOARD_STATE klawiatura;
	ALLEGRO_DISPLAY *okno = al_create_display(640, 600);
	al_set_window_title(okno, "Icy Tower");
	ALLEGRO_BITMAP *obrazek = al_load_bitmap("resources/icytower-bck.png");
	ALLEGRO_BITMAP *obrazek2 = al_load_bitmap("resources/srodek.png");
	ALLEGRO_BITMAP * player_bitmap = al_load_bitmap("resources/Icy_Tower_champ.png");
	ALLEGRO_BITMAP * platform = al_load_bitmap("resources/platform.png");
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
	ALLEGRO_FONT *font = al_load_ttf_font("resources/raidercrusader.ttf", 30, 0);
	al_register_event_source(event_queue, al_get_display_event_source(okno));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	ALLEGRO_BITMAP * menu1 = al_load_bitmap("resources/icytowermenu1.png");
	ALLEGRO_BITMAP * menu2 = al_load_bitmap("resources/icytowermenu2.png");
	ALLEGRO_BITMAP * menu3 = al_load_bitmap("resources/icytowermenu3.png");
	bool move = false;																//Need to go away, works but is not intuitive
	
	ALLEGRO_SAMPLE *sample1 = NULL;
	sample1 = al_load_sample("resources/Icy Tower theme song.wav");
	al_play_sample(sample1, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);

	al_start_timer(timer);

	int menuCounter = 0;

	while (!menu) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (menuCounter < 0)menuCounter += 3;
		menuCounter %= 3;

		switch (menuCounter) {
		case 0:al_draw_bitmap(menu1, 0, 0, 0);
			break;
		case 1:al_draw_bitmap(menu2, 0, 0, 0);
			break;
		case 2:al_draw_bitmap(menu3, 0, 0, 0);
			break;
		}

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) menu = true;
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
			         {
			            al_get_keyboard_state(&klawiatura);
			            if (ev.keyboard.keycode == ALLEGRO_KEY_UP) menuCounter--;
						if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) menuCounter++;
						if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE)
						{
							int button;
							switch (menuCounter)
							{
							case 0:done = false;
								menu = true;
								al_clear_to_color(al_map_rgb(0, 0, 0));

								al_destroy_bitmap(menu1);

								al_destroy_bitmap(menu2);

								al_destroy_bitmap(menu3);
								break;
							case 1:button = al_show_native_message_box(
								okno,
								"Icy Tower (clone)",
								"Projekt na przedmiot programowanie III",
								"Projekt wykonali:\n\nBartosz Ociepka\nBeniamin Stecula\n\nGliwice 2017",
								NULL,
								NULL
							);
								break;
							case 2:menu = true;
							}
						}
				        }
		

		al_flip_display();

		while (!done)
		{
			map.counter++;
			al_get_keyboard_state(&klawiatura);
			al_clear_to_color(al_map_rgb(0, 0, 0));

			al_draw_bitmap_region(obrazek, 0, map.bitmapMove, 640, 600, 0, 0, 0);
			al_draw_bitmap_region(obrazek, 0, 1200 + map.bitmapMove, 640, 600, 0, 0, 0);
			al_draw_bitmap(player_bitmap, player.x, player.y, 0);
			for (std::vector<int>::iterator it = map.YCoordinateIceBlock.begin(), it2 = map.IceBlocksXStart.begin(); it != map.YCoordinateIceBlock.end(), it2 != map.IceBlocksXStart.end(); ++it, ++it2)
			{
				if (*it != 570)
				{
					al_draw_bitmap(platform, *it2, *it, 0);
				}
			}
			std::string x = std::to_string(player.points);
			const char *y = x.c_str();
			al_draw_text(font, al_map_rgb(255, 255, 255), 20, 20, ALLEGRO_ALIGN_CENTRE, y);
			al_flip_display();

			ALLEGRO_EVENT event;
			al_wait_for_event(event_queue, &event);

			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)done = true;
			if (event.type == ALLEGRO_EVENT_TIMER)
			{
				map.MoveCharacter(player, map);
				if (map.bitmapMove < 0)
					obrazek = obrazek2;
				if (player.y < 300 && done1)
				{


					std::vector<int>::iterator it = std::find(map.YCoordinateIceBlock.begin(), map.YCoordinateIceBlock.end(), 570);
					int distance = std::distance(map.YCoordinateIceBlock.begin(), it);
					map.YCoordinateIceBlock.erase(it);
					map.IceBlocksXStart.erase(map.IceBlocksXStart.begin() + distance);
					map.IceBlockLength.erase(map.IceBlockLength.begin() + distance);
					done1 = false;

					map.bitmapMove = 600;

				}

				map.updateSpeed(player, klawiatura, done);

				if (player.y < 300 || move)
				{
					move++;
					if (map.counter % 5 == 0)
						map.MoveMap(map, player, map.PLATFORM_MOVE_VECTOR);
				}

			}

		}

	}

	std::string punkty = "Zdobyles " + std::to_string(player.points) + " punktow.";
	char const *pchar = punkty.c_str();

	int button = al_show_native_message_box(
		okno,
		"Koniec gry",
		"Dobra robota :)",
		pchar,
		NULL,
		NULL
	);

	al_destroy_display(okno);
	al_destroy_bitmap(obrazek);

	return 0;
}

