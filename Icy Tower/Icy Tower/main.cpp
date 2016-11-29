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
	float i = 30;
	while (!al_key_down(&klawiatura, ALLEGRO_KEY_A) && i < 700) //koniec programu gdy wciœniemy klawisz Escape
	{
		al_get_keyboard_state(&klawiatura);  // odczyt stanu klawiatury
		al_clear_to_color(al_map_rgb(0, 255, 0)); // wyczyszczenie aktualnego bufora ekranu
		al_draw_bitmap(obrazek, 0, 0, 0);  // wyœwietlenie bitmapy "obrazek" na "Backbuffer" (bufor ekranu)
		al_draw_bitmap(player_bitmap, i, 530, 0);
		al_flip_display(); // wyœwietlenie aktualnego bufora na ekran
		i += 1;
		Sleep(20);
	}
	// usuwanie z pamiêci okna, bitmap, audio, fontów ...itd.
	al_destroy_display(okno);
	al_destroy_bitmap(obrazek);

	return 0;
}
