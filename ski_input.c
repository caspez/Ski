/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_input.c
 * Description		: Input functions
 -------------------------------------------------------------------------------*/

#include "ski_input.h"

Toggle mouseLeft = OFF, mouseRight = OFF;
int mouseX=0, lastMouseX=0, mouseY=0, lastMouseY=0;
void mouseMove(int x, int y){
#ifdef DEBUG_INPUT
	printf("Mouse x:%i y:%i\n",x,y);
#endif	
	//set mouse values used for rotation and zoom
	lastMouseX = mouseX;
	lastMouseY = mouseY;
	mouseX = x;
	mouseY = y;
}

void mousePassiveMove(int x, int y){
	lastMouseX = mouseX;
	lastMouseY = mouseY;
	mouseX = x;
	mouseY = y;
}

void mouseState(int button, int state, int x, int y){
	if (button == GLUT_LEFT_BUTTON){
		mouseLeft ^=1;
		lastMouseX = mouseX;
		lastMouseY = mouseY;
#ifdef DEBUG_INPUT
		printf("Mouse button: left:%i\n",mouseLeft);
#endif
	}
	if (button == GLUT_RIGHT_BUTTON){
		mouseRight ^=1;
#ifdef DEBUG_INPUT
		printf("Mouse button: right:%i\n",mouseRight);
#endif
	}
}

Toggle viewBinormal = OFF, viewTangent = OFF, viewNormal = OFF;
Toggle forward1=OFF, back1=OFF, left1=OFF, right1=OFF;
Toggle forward2=OFF, back2=OFF, left2=OFF, right2=OFF;
Toggle wireFrame = OFF, viewAxis = OFF, lightScene = ON;
Toggle followHeading = OFF, followOrientation = OFF;
void keyDown(unsigned char key, int x, int y){
#ifdef DEBUG_INPUT
	printf("Keyboard: %c\n",key);
#endif	
	switch(key){
		case 'p':
			wireFrame ^=1;
			break;
		case 'o':
			viewAxis ^=1;
			break;
		case 'n':
			viewNormal ^=1;
			break;
		case 'b':
			viewBinormal ^=1;
			break;
		case 't':
			viewTangent ^=1;
			break;
		case 'l':
			lightScene ^=1;
			break;
		case 'w':
			forward1 ^=1;
			break;
		case 'a':
			left1 ^=1;
			break;
		case 's':
			back1 ^=1;
			break;
		case 'd':
			right1 ^=1;
			break;
		case 'q':
			exitProgram();
			break;
		case 27: //Esc
			exitProgram();
			break;
		case 'h':
			followHeading ^=1;
			break;
		case 'j':
			followOrientation ^=1;
			break;
	}
}

void keyUp(unsigned char key, int x, int y){
#ifdef DEBUG_INPUT
	printf("Keyboard: %c\n",key);
#endif
	switch(key){
		case 'w':
			forward1 ^=1;
			break;
		case 'a':
			left1 ^=1;
			break;
		case 's':
			back1 ^=1;
			break;
		case 'd':
			right1 ^=1;
			break;
	}
}

void arrowKeys(int key, int x, int y){
#ifdef DEBUG_INPUT
	printf("Keyboard: %i\n",key);
#endif	
	switch(key){
		case GLUT_KEY_UP:
			forward2 ^=1;
			break;
		case GLUT_KEY_LEFT:
			left2 ^=1;
			break;
		case GLUT_KEY_DOWN:
			back2 ^=1;
			break;
		case GLUT_KEY_RIGHT:
			right2 ^=1;
			break;
	}
}