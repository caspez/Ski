/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: a1_ski_main.c
 * Description		: Main file of program
 -------------------------------------------------------------------------------*/
#include "ski_main.h"

int window; 
int main(int argc, char **argv){
	generateSlope();//generates slope vertices and normals into an array
	generateTrees();
	generateParticles(ski1.particles.snow);
	generateParticles(ski2.particles.snow);
	setSlopeBoundary(&ski1, 0, X_WAVE_INCREMENT, 0, Z_WAVE_INCREMENT);
	setSlopeBoundary(&ski2, 0, X_WAVE_INCREMENT, 0, Z_WAVE_INCREMENT);
	//orientateSkier();//determine skier's starting orientation
	glutInit(&argc, argv);
	/*output functions*/
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(DEFAULT_WIDTH, DEFAULT_HEIGHT);
	window = glutCreateWindow("Skier");
	glOrtho(-1,1,-1,1,-1,1);
	glEnable(GL_DEPTH_TEST);
	//glDisable(GL_DEPTH_TEST);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape); 

	/*input functions*/
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouseState);
	glutPassiveMotionFunc(mousePassiveMove);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(arrowKeys);
	glutSpecialUpFunc(arrowKeys);

	/*load textures*/
	snowTexture = loadTexture("texture/snow.jpg", ON);
	trees.texture = loadTexture("texture/tree.png", OFF);
	snowballTexture = loadTexture("texture/snowball.png", OFF);
	initSkyBox();

	glutMainLoop();

	return EXIT_SUCCESS;
}

void exitProgram(){
	glutDestroyWindow(window);
	freeSlope();//frees allocated memory for slope vert and norm arrays
	exit (0);
}
