/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_display.h
 * Description		: Display functions header
 -------------------------------------------------------------------------------*/
#ifndef SKI_DISPLAY_H
#define SKI_DISPLAY_H

#include "ski_main.h"

extern int width, height;

void display();
void idle();
void reshape(int width, int height);
void setLighting();
void positionLight();
#endif
