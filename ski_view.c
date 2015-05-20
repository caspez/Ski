/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_view.c
 * Description		: Camera and view functions
 -------------------------------------------------------------------------------*/
#include "ski_view.h"

void drawAxis(){
	if(viewAxis == ON){
		if(lightScene == ON)
			setLighting(OFF);
		glBegin(GL_LINES);
		glColor3f(0,1,0);
		glVertex3f(0,0,0);	
		glVertex3f(0,1,0);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(0,0,0);	
		glVertex3f(1,0,0);
		glEnd();
		glBegin(GL_LINES);
		glColor3f(0,0,1);
		glVertex3f(0,0,0);	
		glVertex3f(0,0,1);
		glEnd();
		if(lightScene == ON)
			setLighting(ON);
	}
}

Camera camera = {
	{CAMERA_X, CAMERA_Y}, //x and y rotation
	CAMERA_ZOOM, //zoom
};
void setCamera(Skier * ski){

	glLoadIdentity();
	glTranslatef(0,0,camera.zoom);

	glRotatef(camera.rotation.x,1,0,0);
	glRotatef(camera.rotation.y,0,1,0);

	//Set camera to follow skier
	//Rotates with heading on cube's y axis
	if(followHeading == ON)
		glRotatef(-ski->rotation.y, 0, 1, 0);

	//Rotates with orientation on cube's z axis and x axis
	if(followOrientation == ON){
		glRotatef(-ski->rotation.x,1,0,0);
		glRotatef(-ski->rotation.z,0,0,1);
	}

	//Camera follows skier's movements
	glTranslatef(-ski->position.x, -ski->position.y, -ski->position.z);

	//Reset the light's position
	positionLight();
}



void setCameraPosition(Skier * ski){
	//BAsed on assignment 1 sample solution
	ski->camera.x += sinf(camera.rotation.y * (float) M_PI / 180.0f) * camera.zoom;
	ski->camera.z += cosf(camera.rotation.y * (float) M_PI / 180.0f) * camera.zoom;
	ski->camera.y += sinf(camera.rotation.x * (float) M_PI / 180.0f) * camera.zoom;
	ski->camera.x = -ski->camera.x;
}


GLuint loadSkyTexture(const char *filename){
	GLuint texture = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (!texture)
		return 0;

	glBindTexture(GL_TEXTURE_2D, texture);
	//hide skybox edges
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}



int skybox[6];
void initSkyBox(){
	skybox[SKYBOX_NEGX] = loadSkyTexture("texture/skybox/negx.bmp");
	skybox[SKYBOX_NEGY] = loadSkyTexture("texture/skybox/negy.bmp");
	skybox[SKYBOX_NEGZ] = loadSkyTexture("texture/skybox/negz.bmp");
	skybox[SKYBOX_POSX] = loadSkyTexture("texture/skybox/posx.bmp");
	skybox[SKYBOX_POSY] = loadSkyTexture("texture/skybox/posy.bmp");
	skybox[SKYBOX_POSZ] = loadSkyTexture("texture/skybox/posz.bmp");

}

float skyboxSize = 1;
void drawSkybox(){
	//Set camera orientation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Rotate camera inside skybox
	glRotatef(camera.rotation.x,1,0,0);
	glRotatef(camera.rotation.y,0,1,0);

	setLighting(OFF);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, skybox[SKYBOX_POSX]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(-skyboxSize,-skyboxSize,-skyboxSize);
	glTexCoord2f(1,0);
	glVertex3f(+skyboxSize,-skyboxSize,-skyboxSize);
	glTexCoord2f(1,1);
	glVertex3f(+skyboxSize,+skyboxSize,-skyboxSize);
	glTexCoord2f(0,1);
	glVertex3f(-skyboxSize,+skyboxSize,-skyboxSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, skybox[SKYBOX_NEGX]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(+skyboxSize,-skyboxSize,+skyboxSize);
	glTexCoord2f(1,0);
	glVertex3f(-skyboxSize,-skyboxSize,+skyboxSize);
	glTexCoord2f(1,1);
	glVertex3f(-skyboxSize,+skyboxSize,+skyboxSize);
	glTexCoord2f(0,1);
	glVertex3f(+skyboxSize,+skyboxSize,+skyboxSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, skybox[SKYBOX_POSZ]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(+skyboxSize,-skyboxSize,-skyboxSize);
	glTexCoord2f(1,0);
	glVertex3f(+skyboxSize,-skyboxSize,+skyboxSize);
	glTexCoord2f(1,1);
	glVertex3f(+skyboxSize,+skyboxSize,+skyboxSize);
	glTexCoord2f(0,1);
	glVertex3f(+skyboxSize,+skyboxSize,-skyboxSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, skybox[SKYBOX_NEGZ]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(-skyboxSize,-skyboxSize,+skyboxSize);
	glTexCoord2f(1,0);
	glVertex3f(-skyboxSize,-skyboxSize,-skyboxSize);
	glTexCoord2f(1,1);
	glVertex3f(-skyboxSize,+skyboxSize,-skyboxSize);
	glTexCoord2f(0,1);
	glVertex3f(-skyboxSize,+skyboxSize,+skyboxSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, skybox[SKYBOX_POSY]);
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(-skyboxSize,+skyboxSize,-skyboxSize);
	glTexCoord2f(1,0);
	glVertex3f(+skyboxSize,+skyboxSize,-skyboxSize);
	glTexCoord2f(1,1);
	glVertex3f(+skyboxSize,+skyboxSize,+skyboxSize);
	glTexCoord2f(0,1);
	glVertex3f(-skyboxSize,+skyboxSize,+skyboxSize);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, skybox[SKYBOX_NEGY]);
	glBegin(GL_QUADS);
	glTexCoord2f(1,1);
	glVertex3f(+skyboxSize,-skyboxSize,-skyboxSize);
	glTexCoord2f(0,1);
	glVertex3f(-skyboxSize,-skyboxSize,-skyboxSize);
	glTexCoord2f(0,0);
	glVertex3f(-skyboxSize,-skyboxSize,+skyboxSize);
	glTexCoord2f(1,0);
	glVertex3f(+skyboxSize,-skyboxSize,+skyboxSize);
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	if(lightScene == ON)
		setLighting(ON);
}