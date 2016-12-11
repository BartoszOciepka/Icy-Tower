#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <windows.h>
#include <string>
#include <cmath>
#include "Player.h"

int main()
{ //		************CONTROL PANEL ************************
	const int RIGHT_END = 537;
	const int LEFT_END = 50;
	const float SPEED_SLOWDOWN = 0.7;
	const float SPEED_INCREASE = 0.7;
	const float FPS = 60;
	Player player;

	al_init(); // inicjowanie biblioteki allegro
	al_install_keyboard(); // instalowanie sterownika klawiatury
	al_init_image_addon();// inicjowanie dodatku umo¿liwiaj¹cego odczyt jak i zapis obrazów, w formatach BMP,  PNG, JPG, PCX, TGA.
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_KEYBOARD_STATE klawiatura; // utworzenie struktury do odczytu stanu klawiatury
	ALLEGRO_DISPLAY *okno = al_create_display(640, 600);// tworzymy wskaŸnik okna, i podajemy jego szer. i wys
	ALLEGRO_FONT *font = al_load_ttf_font("raidercrusader.ttf", 72, 0);
	al_set_window_title(okno, "Icy Tower");// podajemy tytu³ okna
	ALLEGRO_BITMAP *obrazek = al_load_bitmap("icytower-bck.png");// wczytujemy bitmapê do pamiêci
	//player-> 
	ALLEGRO_BITMAP * player_bitmap = al_load_bitmap("champ.png");
	int i = 30;
	bool done = false;
	bool jump = false;
	float end_y = 0;
	
	ALLEGRO_TIMER *timer = al_create_timer(1.0 / FPS);
	ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(okno));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_start_timer(timer);
	
	while (!done) //koniec programu gdy wciœniemy klawisz Escape
	{
		al_get_keyboard_state(&klawiatura);  // odczyt stanu klawiatury
		al_clear_to_color(al_map_rgb(0, 255, 0)); // wyczyszczenie aktualnego bufora ekranu
		al_draw_bitmap(obrazek, 0, 0, 0);  // wyœwietlenie bitmapy "obrazek" na "Backbuffer" (bufor ekranu)
		al_draw_bitmap(player_bitmap, player.x, player.y, 0);
		std::string x = std::to_string(player.speed);
		const char *y = x.c_str();
		al_draw_text(font, al_map_rgb(255, 255, 255), 640 / 2, (480 / 4), ALLEGRO_ALIGN_CENTRE, y);
		al_flip_display(); // wyœwietlenie aktualnego bufora na ekran
		//i += 5;
		//Sleep(20);

		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)done = true;
		if (event.type == ALLEGRO_EVENT_TIMER) 
		{
			al_get_keyboard_state(&klawiatura);
			if (player.x == RIGHT_END) {
				player.speed = -player.speed - 1;
				player.x += player.speed;
			}
			else if (player.x == LEFT_END) {
				player.speed = -player.speed + 1;
				player.x += player.speed;
			}
			else if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && player.x < RIGHT_END && player.x > LEFT_END) {
				player.x += player.speed;
				if (player.x > RIGHT_END)player.x = RIGHT_END;
				if(player.speed < 10)player.speed += SPEED_INCREASE;
			}
			else if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && player.speed > 0 && player.x < RIGHT_END) {
				player.x += player.speed;
				if (player.x > RIGHT_END)player.x = RIGHT_END;
			}
			else if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT))player.speed += SPEED_INCREASE;


			else if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && player.x > LEFT_END && player.x < RIGHT_END) {
				player.x += player.speed;
				if (player.x < LEFT_END)player.x = LEFT_END;
				if(player.speed > -10)player.speed -= SPEED_INCREASE;
			}
			else if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT) && player.speed < 0 && player.x > LEFT_END) {
				player.x += player.speed;
				if (player.x < LEFT_END)player.x = LEFT_END;
			}
			else if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT))player.speed -= SPEED_INCREASE;

			else if (al_key_down(&klawiatura, ALLEGRO_KEY_UP) && player.vertical_speed == 0) {
				player.vertical_speed = 4;
				end_y = player.y - player.jump_height;
				jump = true;
			}


			else if (abs(player.speed < 1) && player.x > RIGHT_END) {
				player.x = RIGHT_END;
				player.speed = -abs(2 * player.speed);
			}
			else if (abs(player.speed) < 1)player.speed = 0;
			else if (player.speed > 0 && player.x < RIGHT_END)
			{
				player.speed -= SPEED_SLOWDOWN;
				player.x += player.speed;
				if (player.x > RIGHT_END)player.x = RIGHT_END;
			}
			else if (player.speed > 0 && player.x >= RIGHT_END)player.speed = 0;
			else if (player.speed < 0 && player.x > LEFT_END)
			{
				player.speed += SPEED_SLOWDOWN;
				player.x += player.speed;
				if (player.x < LEFT_END)player.x = LEFT_END;
			}
			else if (player.speed < 0 && player.x <= LEFT_END)player.speed = 0;

			if (player.y <= end_y) {
				jump = false;
				player.vertical_speed = 0;
			}

			if (jump) {
				player.y -= player.vertical_speed;
			}

			
		}
		

		
		
	}
	// usuwanie z pamiêci okna, bitmap, audio, fontów ...itd.
	al_destroy_display(okno);
	al_destroy_bitmap(obrazek);

	return 0;
}
