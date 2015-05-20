/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_slope.h
 * Description		: Slope functions header
 -------------------------------------------------------------------------------*/
#ifndef SKI_SLOPE_H
#define SKI_SLOPE_H

#include "ski_main.h"

extern int slopeVCount;
extern float * slopeVerts;
extern float xRotate, yRotate, zoom;
extern float xCube, yCube, zCube, skiHeading, skiOrientation;
extern GLuint snowTexture;
extern float xWaveTranslate, zWaveTranslate;

float getSine(float x, float z);
float getDxySine(float x);
float getDzySine(float z);
void generateSlope();
void drawSlope();
void drawPlayerSlope(Skier * ski);
void setSlopeBoundary(Skier * ski, float minX, float maxX, float minZ, float maxZ);
void freeSlope();
#endif
