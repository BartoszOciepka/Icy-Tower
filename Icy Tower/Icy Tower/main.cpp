#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <windows.h>
#include "Player.h"

int main()
{
	Player player;
	al_init(); // inicjowanie biblioteki allegro
	al_install_keyboard(); // instalowanie sterownika klawiatury
	al_init_image_addon();// inicjowanie dodatku umo�liwiaj�cego odczyt jak i zapis obraz�w, w formatach BMP,  PNG, JPG, PCX, TGA.
	ALLEGRO_KEYBOARD_STATE klawiatura; // utworzenie struktury do odczytu stanu klawiatury
	ALLEGRO_DISPLAY *okno = al_create_display(640, 600);// tworzymy wska�nik okna, i podajemy jego szer. i wys
	al_set_window_title(okno, "Icy Tower");// podajemy tytu� okna
	ALLEGRO_BITMAP *obrazek = al_load_bitmap("icytower-bck.png");// wczytujemy bitmap� do pami�ci
	//player-> 
	ALLEGRO_BITMAP * player_bitmap = al_load_bitmap("champ.png");
	float i = 30;
	while (!al_key_down(&klawiatura, ALLEGRO_KEY_A) && i < 700) //koniec programu gdy wci�niemy klawisz Escape
	{
		al_get_keyboard_state(&klawiatura);  // odczyt stanu klawiatury
		al_clear_to_color(al_map_rgb(0, 255, 0)); // wyczyszczenie aktualnego bufora ekranu
		al_draw_bitmap(obrazek, 0, 0, 0);  // wy�wietlenie bitmapy "obrazek" na "Backbuffer" (bufor ekranu)
		al_draw_bitmap(player_bitmap, i, 530, 0);
		al_flip_display(); // wy�wietlenie aktualnego bufora na ekran
		i += 1;
		Sleep(20);
	}
	// usuwanie z pami�ci okna, bitmap, audio, font�w ...itd.
	al_destroy_display(okno);
	al_destroy_bitmap(obrazek);

	return 0;
}
