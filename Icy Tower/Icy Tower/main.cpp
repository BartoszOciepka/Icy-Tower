#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

int main(int argc, char **argv)
{
	al_init();
	al_install_keyboard();
	al_init_image_addon();
	ALLEGRO_KEYBOARD_STATE klawiatura;
	ALLEGRO_DISPLAY *okno = al_create_display(800, 600);
	al_set_window_title(okno, "Icy Tower - projekt na programowanie III");
	ALLEGRO_BITMAP *obrazek = al_load_bitmap("widok.png");
	while (!al_key_down(&klawiatura, ALLEGRO_KEY_A))
	{
		al_get_keyboard_state(&klawiatura);
		al_clear_to_color(al_map_rgb(255, 255, 255));
		al_draw_bitmap(obrazek, 0, 0, 0);
		al_flip_display();
	}

	al_destroy_display(okno);
	al_destroy_bitmap(obrazek);
	return 0;
}