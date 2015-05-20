/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_display.c
 * Description		: Display functions
 -------------------------------------------------------------------------------*/
#include "ski_display.h"

void drawScene(Skier * playerA, Skier * playerB){
	// set polygon mode
	if(wireFrame == ON )
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	drawSkybox();
	setLighting(lightScene);
	setCamera(playerA);	
	drawAxis();
	drawSkier(playerA);
	drawSkier(playerB);

	drawPlayerSlope(playerA);
	drawPlayerTrees(playerA);
	//drawTrees(playerA);
	if(DRAW_PARTICLES == 1){
		drawParticles(playerB);
		drawParticles(playerA);
	}
}
void printHUDText(int x, int y, char *string){
	int len = (int) strlen(string);
	int offset = 96;
	glRasterPos2f(x-offset,y);
	int i;
	for(i = 0; i < len; i++){
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,string[i]);
	}
}

int lostGame = 0;
void drawHUD(){
	int screenYLine = 20;
	int player1Area = width/4;
	int player2Area = player1Area*3;

	if(lightScene == ON)
		setLighting(OFF);
	glColor3f(0.0, 1.0, 0.0);

	char timeString[30];
	float elapasedTime = glutGet(GLUT_ELAPSED_TIME);
	elapasedTime /= 1000.0;
	float timeLeft = GAME_TIME - elapasedTime;
	if(timeLeft <= 0 || lostGame > 0){
		//if game ended due to collisions
		if(lostGame == 1){
			sprintf(timeString,"Player 1 Loses!");
		}
		else if(lostGame == 2){
			sprintf(timeString,"Player 2 Loses!");
		}
		else if (ski1.airTime > ski2.airTime){
			sprintf(timeString,"Player 1 Wins!");
		}
		else if (ski1.airTime < ski2.airTime){
			sprintf(timeString,"Player 2 Wins!");
		}
		else
			sprintf(timeString,"        Draw!");
	}
	else{
		sprintf(timeString,"  Time Left: %f",timeLeft);
	}

	printHUDText(player1Area*2, screenYLine, timeString);

	printHUDText(player1Area, screenYLine, "Player 1");
	printHUDText(player2Area, screenYLine, "Player 2");

	char airTime[30];
	sprintf(airTime, "Air time: %f",ski1.airTime);
	printHUDText(player1Area, screenYLine*2, airTime);
	sprintf(airTime, "Air time: %f",ski2.airTime);
	printHUDText(player2Area, screenYLine*2, airTime);

	
	char collisions[20];
	sprintf(collisions, "Collisions: %i",ski1.collision.count);
	printHUDText(player1Area, screenYLine*3, collisions);
	sprintf(collisions, "Collisions: %i",ski2.collision.count);
	printHUDText(player2Area, screenYLine*3, collisions);

	glColor3f(1,1,1);

	if(lightScene == ON)
		setLighting(ON);
}

void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*Render player 1 viewport*/
	glViewport(0, 0, width/2, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75,(float)width/2/(float)height,0.01,100);
	glMatrixMode(GL_MODELVIEW);
	drawScene(&ski1,&ski2);

	/*Render player 2 viewport*/
	glViewport (width/2, 0, width/2, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75,(float)width/2/(float)height,0.01,100);
	glMatrixMode(GL_MODELVIEW);
	drawScene(&ski2,&ski1);

	/*Render HUD*/
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, height, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawHUD();

	const char * error = (char*)gluErrorString(glGetError());
	if (strcmp(error,"no error")!=0)
		printf("%s\n",error);
	glutSwapBuffers();
}

void idle(){
	float time, dTime;
	static float tLast = 0.0;

	/*get time and convert to seconds*/
	time = glutGet(GLUT_ELAPSED_TIME);
	time /= 1000.0;

	/*calculate delta time*/
	dTime = time-tLast;

	/*Update tLast for next round, using static local*/
	tLast = time;

	/*rotate camera*/
	float mouseRotateV = 50;
	if(mouseLeft == ON){
		//checks if mouse has been moved before setting new rotate value
		//if mouseLeft=ON yet there physical mouse is still
		//there may be a difference of 1 even if mouse has not been changed
		//since mouseMove has not been updated
		if(abs(lastMouseX-mouseX)>1)
			camera.rotation.y += (mouseX - lastMouseX) * dTime * mouseRotateV;
		if(abs(lastMouseY-mouseY)>1)
			camera.rotation.x += (mouseY - lastMouseY) * dTime * mouseRotateV;
#ifdef DEBUG_INPUT
		printf("xRot=%f,yRot=%f\n",camera.rotation.x,camera.rotation.y);
#endif
	}

	/*zoom camera*/
	float mouseZoomV = 3;
	if(mouseRight == ON){
		if(abs(lastMouseY-mouseY)>1)
			camera.zoom -= (mouseY-lastMouseY) * dTime * mouseZoomV;
	}

	//Disable movement if game ended
	if(time < GAME_TIME && lostGame == 0){
		moveSkier(&ski1, dTime, forward1, back1, left1, right1);
		moveSkier(&ski2, dTime, forward2, back2, left2, right2);
		moveParticles(ski1.particles.snow, dTime, time, &ski1.particles.elapsedTime);
		moveParticles(ski2.particles.snow, dTime, time, &ski2.particles.elapsedTime);
		if(checkForCollisionWrap(&ski1) > 0){
			lostGame = 1;
		}
		if(checkForCollisionWrap(&ski2) > 0){
			lostGame = 2;
		}
	}

	glutPostRedisplay();
}

int width = DEFAULT_WIDTH;
int height = DEFAULT_HEIGHT;
void reshape(int newWidth, int newHeight){
	width = newWidth;
	height = newHeight;
	glViewport(0,0,width,height);
	glutPostRedisplay();
}

void setLighting(Toggle lightToggle){
	if(lightToggle == ON){
		//enable lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glShadeModel(GL_SMOOTH);
	}
	if(lightToggle == OFF){
		glDisable(GL_LIGHTING);
		glDisable(GL_LIGHT0);
	}
}

//Position the light
void positionLight(){
	float position[] = {0.9,2.4,0.6,1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, position);
}