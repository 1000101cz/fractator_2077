#include <stdio.h>
#include "messages.h"
#include "../system/terminal_colors.h"

void welcome_message(int language)
{
  switch (language){
    case 0: // EN
    	green_col();
    	fprintf(stderr,
    		"FRACTATOR 2077 - seminar work\n  created by Marousek Stepan 'maroust1'\n  2021 06 15\n  Czech Technical University in Prague\n");
    	def_color();
    	fprintf(stderr, "\n*************************************************\n");
    	fprintf(stderr, "Controls:                                       *\n");
    	fprintf(stderr, "  basic:                                        *\n");
    	fprintf(stderr, "   c    compute fractal                         *\n");
    	fprintf(stderr, "   0    play animation                          *\n");
    	fprintf(stderr, "- - - - - - - - - - - - - - - - - - - - - - - - *\n");
    	fprintf(stderr, "  movement:                                     *\n");
    	fprintf(stderr, "   8    move over the picture (UP)              *\n");
    	fprintf(stderr, "   2    move over the picture (DOWN)            *\n");
    	fprintf(stderr, "   4    move over the picture (LEFT)            *\n");
    	fprintf(stderr, "   6    move over the picture (RIGHT)           *\n");
    	fprintf(stderr, "   u    zoom in                                 *\n");
    	fprintf(stderr, "   j    zoom out                                *\n");
    	fprintf(stderr, "   h    decrease c number values                *\n");
    	fprintf(stderr, "   k    increase c number values                *\n");
    	fprintf(stderr, "   7    decrease number of iterations           *\n");
    	fprintf(stderr, "   9    increase number of iterations           *\n");
    	fprintf(stderr, "- - - - - - - - - - - - - - - - - - - - - - - - *\n");
    	fprintf(stderr, "  window controls:                              *\n");
    	fprintf(stderr, "   l    erase actual picture buffer             *\n");
    	fprintf(stderr, "   p    display current buffer (refresh)        *\n");
    	fprintf(stderr, "   +    resize window (+)                       *\n");
    	fprintf(stderr, "   -    resize window (-)                       *\n");
    	fprintf(stderr, "- - - - - - - - - - - - - - - - - - - - - - - - *\n");
    	fprintf(stderr, "   q    exit application                        *\n");
    	fprintf(stderr, "*************************************************\n");
    	fprintf(stderr, "\n");
      break;

    case 1: // CZ
      green_col();
      fprintf(stderr,
        "FRACTATOR 2077 - seminární práce\n  vytvořil Marousek Štěpán 'maroust1'\n  2021 06 15\n  ČVUT v Praze\n");
      def_color();
      fprintf(stderr, "\n*************************************************\n");
      fprintf(stderr, "Ovládání:                                       *\n");
      fprintf(stderr, "  základní:                                     *\n");
      fprintf(stderr, "   c    vypočítat fraktál                       *\n");
      fprintf(stderr, "   0    přehrát animaci                         *\n");
      fprintf(stderr, "- - - - - - - - - - - - - - - - - - - - - - - - *\n");
      fprintf(stderr, "  pohyb:                                        *\n");
      fprintf(stderr, "   8    pohyb nad obrázkem (UP)                 *\n");
      fprintf(stderr, "   2    pohyb nad obrázkem (DOWN)               *\n");
      fprintf(stderr, "   4    pohyb nad obrázkem (LEFT)               *\n");
      fprintf(stderr, "   6    pohyb nad obrázkem (RIGHT)              *\n");
      fprintf(stderr, "   u    přiblížit                               *\n");
      fprintf(stderr, "   j    oddálit                                 *\n");
      fprintf(stderr, "   h    zmenšit číslo c                         *\n");
      fprintf(stderr, "   k    zvětšit číslo c                         *\n");
      fprintf(stderr, "   7    snížit počet iterací                    *\n");
      fprintf(stderr, "   9    zvýšit počet iterací                    *\n");
      fprintf(stderr, "- - - - - - - - - - - - - - - - - - - - - - - - *\n");
      fprintf(stderr, "  ovládání okna:                                *\n");
      fprintf(stderr, "   l    smazat aktuální  buffer                 *\n");
      fprintf(stderr, "   p    zobrazit aktuální buffer (refresh)      *\n");
      fprintf(stderr, "   +    zvětšit okno                            *\n");
      fprintf(stderr, "   -    zmenšit okno                            *\n");
      fprintf(stderr, "- - - - - - - - - - - - - - - - - - - - - - - - *\n");
      fprintf(stderr, "   q    odejít                                  *\n");
      fprintf(stderr, "*************************************************\n");
      fprintf(stderr, "\n");
  }
}

void print_enter_x_size(int language)
{
  switch (language) {
    case 0: // EN
      printf("Enter x axis size: ");
      break;

    case 1: // CZ
      printf("Zadej šířku: ");
      break;

  }
}

void print_enter_y_size(int language)
{
  switch (language) {
    case 0: // EN
      printf("Enter y axis size: ");
      break;

    case 1: // CZ
      printf("Zadej výšku: ");
      break;

  }
}

void print_saving_enabled(int language)
{
  switch (language) {
    case 0: // EN
      printf("Saving generated pictures enabled\n");
      break;

    case 1: // CZ
      printf("Ukládání generovaných obrázků povoleno\n");
      break;

  }
}

void print_audio_animation(int language)
{
  switch (language) {
    case 0: // EN
      printf("Audio animation ON\n");
      break;

    case 1: // CZ
      printf("Zvuková animace zapnuta\n");
      break;

  }
}

void print_prediction_precision(int language)
{
  switch (language) {
    case 0: // EN
      printf("Choose prediction precision [(0-5) / (10-13)]: ");
      break;

    case 1: // CZ
      printf("Zvolte přesnost predikce [(0-5) / (10-13)]: ");
      break;

  }
}

void print_invalid_value(int language)
{
  switch (language) {
    case 0: // EN
      red_col();
      fprintf(stderr,"ERROR: Invalid value!\n");
      break;

    case 1: // CZ
      red_col();
      fprintf(stderr,"CHYBA: Neplatná hodnota!\n");
      break;

  }
}

void print_distance_pixels(int language)
{
  switch (language) {
    case 0: // EN
      printf("Choose distance between pixels: ");
      break;

    case 1: // CZ
      printf("Zvolte vzdálenost mezi pixely: ");
      break;

  }
}

void print_option_needs_value(int language)
{
  switch (language) {
    case 0: // EN
      fprintf(stderr,"Option needs a value\n");
      break;

    case 1: // CZ
      fprintf(stderr,"Parametr potřebuje hodnotu\n");
      break;

  }
}

void print_unknown_argument(int language, char optopt)
{
  switch (language) {
    case 0: // EN
      printf("Unknown argument: %c\n", optopt);
      break;

    case 1: // CZ
      printf("Neznámý argument: %c\n", optopt);
      break;

  }
}
