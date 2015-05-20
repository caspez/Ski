/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_billboards.h
 * Description		: Billboards (trees)
 -------------------------------------------------------------------------------*/
#ifndef SKI_BILLBOARDS_H
#define SKI_BILLBOARDS_H

#include "ski_main.h"

extern Trees trees;
extern GLuint snowballTexture;

/*void drawTrees();*/
void generateTrees();
void drawTrees(Skier * ski);

void generateParticles(Particle * snow);
void drawParticles(Skier * ski);
void moveParticles(Particle * snow, float dTime, float elapasedTime, float * particleTime);
void drawPlayerTrees(Skier * ski);
#endif