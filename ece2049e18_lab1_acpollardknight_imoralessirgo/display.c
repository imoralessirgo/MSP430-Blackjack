/*
 * display.c
 *
 *  Created on: May 31, 2018
 *      Author: imoralessirgo
 */

#include "display.h"
#include <msp430.h>
#include "peripherals.h"

void make_string(char suit,char rank,int x,int y)
{
 unsigned char str[4]; // Declare a buffer of 4 chars, which we will fill
 str[0] = suit;
 str[1] = '-';
 str[2] = rank;
 str[3] = '\0';

 Graphics_drawStringCentered(&g_sContext, str, AUTO_STRING_LENGTH,  x, y, TRANSPARENT_TEXT);

}



