/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_skier.h
 * Description		: Input functions header
 -------------------------------------------------------------------------------*/
#ifndef SKI_SKIER_H
#define SKI_SKIER_H

#include "ski_main.h"

extern Skier ski1, ski2;

Toggle isAirborne(Skier * ski);
void drawSkier(Skier * ski);
void orientateSkier(Skier * ski);
void moveSkier(Skier * ski, float dTime, Toggle forward, Toggle back, Toggle left, Toggle right);
#endif
