#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <windows.h>
#include "Player.h"

int main()
{
	Player player;

	al_init(); // inicjowanie biblioteki allegro
	al_install_keyboard(); // instalowanie sterownika klawiatury
	al_init_image_addon();// inicjowanie dodatku umo¿liwiaj¹cego odczyt jak i zapis obrazów, w formatach BMP,  PNG, JPG, PCX, TGA.
	ALLEGRO_KEYBOARD_STATE klawiatura; // utworzenie struktury do odczytu stanu klawiatury
	ALLEGRO_DISPLAY *okno = al_create_display(640, 600);// tworzymy wskaŸnik okna, i podajemy jego szer. i wys
	al_set_window_title(okno, "Icy Tower");// podajemy tytu³ okna
	ALLEGRO_BITMAP *obrazek = al_load_bitmap("icytower-bck.png");// wczytujemy bitmapê do pamiêci
	//player-> 
	ALLEGRO_BITMAP * player_bitmap = al_load_bitmap("champ.png");
	int i = 30;
	bool done = false;
	const float FPS = 60;
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
		al_flip_display(); // wyœwietlenie aktualnego bufora na ekran
		//i += 5;
		Sleep(20);

		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)done = true;
		if (event.type == ALLEGRO_EVENT_TIMER) 
		{
			al_get_keyboard_state(&klawiatura);
			if (al_key_down(&klawiatura, ALLEGRO_KEY_RIGHT) && player.x <570)player.x += 5;
			if (al_key_down(&klawiatura, ALLEGRO_KEY_LEFT))player.x -= 5;
		}
		

		
		
	}
	// usuwanie z pamiêci okna, bitmap, audio, fontów ...itd.
	al_destroy_display(okno);
	al_destroy_bitmap(obrazek);

	return 0;
}
