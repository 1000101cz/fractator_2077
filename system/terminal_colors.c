/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    2021/07/24                        *
 **********************************************/

#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "terminal_colors.h"

/* functions for print color change */
void green_col()
{
	fprintf(stderr, "\033[1;32m");
}

void red_col()
{
	fprintf(stderr, "\033[1;31m");
}

void yellow_col()
{
	fprintf(stderr, "\033[1;33m");
}

void def_color()
{
	fprintf(stderr, "\033[0m");
}

/* manage terminal settings */
void call_termios(int reset)
{
	static struct termios tio, tioOld;
	tcgetattr(STDIN_FILENO, &tio);

	if (reset) {
		tcsetattr(STDIN_FILENO, TCSANOW, &tioOld);

	} else {
		tioOld = tio;	//backup
		cfmakeraw(&tio);
		tio.c_oflag |= OPOST;
		tcsetattr(STDIN_FILENO, TCSANOW, &tio);
	}
}
