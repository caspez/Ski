/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_input.h
 * Description		: Input functions header
 -------------------------------------------------------------------------------*/
#ifndef SKI_INPUT_H
#define SKI_INPUT_H

#include "ski_main.h"

//Mouse Toggles
extern Toggle mouseLeft, mouseRight;
extern int mouseX, lastMouseX, mouseY, lastMouseY;

//Keyboard Toggles
extern Toggle viewBinormal, viewTangent, viewNormal;
extern Toggle forward1, back1, left1, right1;
extern Toggle forward2, back2, left2, right2;
extern Toggle wireFrame, lightScene, viewAxis;
extern Toggle followHeading, followOrientation;

void mouseMove(int x, int y);
void mousePassiveMove(int x, int y);
void mouseState(int button, int state, int x, int y);

void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void arrowKeys(int key, int x, int y);
#endif
