/**********************************************
 * name:    FRACTATOR 2077                    *
 * author:  STEPAN MAROUSEK                   *
 * date:    05/16/2021                        *
 **********************************************/

#include <stdio.h>

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
