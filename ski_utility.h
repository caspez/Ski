/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_utility.h
 * Description		: Utility Functions
 -------------------------------------------------------------------------------*/

#ifndef SKI_UTILITY_H
#define SKI_UTILITY_H

#include "ski_main.h"

GLuint loadTexture(const char *filename, Toggle toggle);
void initRandomSeed();
int checkForCollisionWrap(Skier * ski);
#endif
