/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_view.h
 * Description		: Camera and view functions header
 -------------------------------------------------------------------------------*/
#ifndef SKI_VIEW_H
#define SKI_VIEW_H

#include "ski_main.h"

// Skybox Texture constants
#define SKYBOX_NEGX 0
#define SKYBOX_NEGY 1
#define SKYBOX_NEGZ 2
#define SKYBOX_POSX 3
#define SKYBOX_POSY 4
#define SKYBOX_POSZ 5

extern Camera camera;

void drawAxis();
void setCamera(Skier * ski);
void initSkyBox();
void drawSkybox();
void setCameraPosition(Skier * ski);
#endif
