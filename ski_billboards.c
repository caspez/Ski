/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_billboards.c
 * Description		: Billboards (trees)
 -------------------------------------------------------------------------------*/
#include "ski_billboards.h"

Trees trees = {
	0,//trees.texture
	TREE_RADIUS, //trees.radius
	TREE_COUNT, //trees.count
	0, //trees.coordCount
	//trees.coordinates
};

void generateTrees(){
	/*Allocate enough memory for treecount number of tree coordinates*/
	trees.coordinates = (Vector3f*) malloc(trees.count*4*sizeof(Vector3f));
	if(trees.coordinates == NULL){
		printf("Failed to allocate memory for tree coordinates!\n");
		exit(EXIT_FAILURE);
	}
	
	initRandomSeed();

	int counter;
	for(counter = 0; counter < trees.count; counter++){
		//Get random value between x min and x max 
		trees.coordinates[counter].x = slopeVerts[0] + (float) (rand()) /( (float) (RAND_MAX/(slopeVerts[slopeVCount-3] - slopeVerts[0])));
		/*x = rand() % slopeVerts[slopeVCount-3] + slopeVerts[0];*/

		//Get random value between z min and z max
		/*z = rand() % slopeVerts[slopeVCount-1] + slopeVerts[2];*/
		trees.coordinates[counter].z = slopeVerts[2] + (float) (rand()) /( (float) (RAND_MAX/(slopeVerts[slopeVCount-1]- slopeVerts[2])));
		trees.coordinates[counter].y =  getSine(trees.coordinates[counter].x, trees.coordinates[counter].z);
	}	
}

//based on following:
//http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat1
void startBillboard() {
	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	for(i=0;i<3;i+=2){
		for(j=0;j<3;j++){
			if(i==j)
				modelview[i*4+j]=1.0;
			else
				modelview[i*4+j]=0.0;
		}
	}


	// set the modelview matrix
	glLoadMatrixf(modelview);
}


void drawTexQuad(float size){
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex3f(0,0,0);
	glTexCoord2f(1,0);
	glVertex3f(size,0,0);
	glTexCoord2f(1,1);
	glVertex3f(size,size,0);
	glTexCoord2f(0,1);
	glVertex3f(0,size,0);
	glEnd();
}

Vector3f cameraPosition = {0,0,0};
int compareCamDistance(const void * a, const void * b){
	Vector3f treeA = *(Vector3f *)a;
	Vector3f treeB = *(Vector3f *)b;
	float distanceA = sqrt((cameraPosition.x-treeA.x)*(cameraPosition.x-treeA.x)
						+ (cameraPosition.y-treeA.y)*(cameraPosition.y-treeA.y)
						+ (cameraPosition.z-treeA.z)*(cameraPosition.z-treeA.z));
	float distanceB = sqrt((cameraPosition.x-treeB.x)*(cameraPosition.x-treeB.x)
					+ (cameraPosition.y-treeB.y)*(cameraPosition.y-treeB.y)
					+ (cameraPosition.z-treeB.z)*(cameraPosition.z-treeB.z));
	//If larger distance then render before
	if(distanceA > distanceB)
		return -1;
	//If smaller distance then render before
	else if(distanceA < distanceB)
		return 1;

	return 0;
}
void drawTrees(Skier * ski){
	if(TREE_DEPTH_SORT == 0){
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.5);
	}
	else{
		//Sort coordinates in reverse order of depth
		//printf("camera x:%f y:%f z:%f ", ski->camera.x, ski->camera.y, ski->camera.z);
		setCameraPosition(ski);
		cameraPosition.x = ski->camera.x;
		cameraPosition.y = ski->camera.y;
		cameraPosition.z = ski->camera.z;
		//printf(" camera x:%f y:%f z:%f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);

		qsort(trees.coordinates, trees.count, sizeof(Vector3f), compareCamDistance);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}



	if(lightScene == ON)
		setLighting(OFF);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, trees.texture);

	
	int i = 0;
	int tree;
	for(tree = 0; tree < trees.count; tree++){
		glPushMatrix();
			glTranslatef(trees.coordinates[tree].x, trees.coordinates[tree].y, trees.coordinates[tree].z);
			i+=3;
			startBillboard();
				drawTexQuad(1);
			glPopMatrix();
		glPopMatrix();
	}
	
	if(lightScene == ON)
		setLighting(ON);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

}

GLuint snowballTexture = 0;
void moveParticles(Particle * snow, float dTime, float elapasedTime, float * particleTime){
	int i;
	/*if under 5 seconds since initial particle movement*/
	if((*particleTime + PARTICLE_REGEN) > elapasedTime){
		for(i = 0; i < PARTICLE_COUNT; i++){
			snow[i].position.x+= snow[i].direction.x * snow[i].speed * dTime;
			snow[i].position.y+= snow[i].direction.y * snow[i].speed * dTime;
			snow[i].position.z+= snow[i].direction.z * snow[i].speed * dTime;
		}
	}
	else{
		*particleTime = elapasedTime;
		generateParticles(snow);
		/* reset particle position*/
		for(i = 0; i < PARTICLE_COUNT; i++){
			snow[i].position.x = snow[i].firstPosition.x;
			snow[i].position.y = snow[i].firstPosition.y;
			snow[i].position.z = snow[i].firstPosition.z;
		}
	}
}

void generateParticles(Particle * snow){
	float maxPosition = PARTICLE_MAX_POSITION;
	float minPosition = maxPosition-(2.0*maxPosition);
	float maxDirectionX = PARTICLE_MAX_DIRECTION_X;
	float minDirectionX = maxDirectionX-(2.0*maxDirectionX);
	float maxDirectionY = PARTICLE_MAX_DIRECTION_Y;
	float minDirectionY = maxDirectionX-(2.0*maxDirectionY);
	float maxParticleSpeed = PARTICLE_MAX_SPEED;
	int i;

	initRandomSeed();
	for(i = 0; i < PARTICLE_COUNT; i++){
		snow[i].position.x = minPosition + (float) (rand()) /((float) (RAND_MAX/(maxPosition*2)));
		snow[i].position.y = minPosition + (float) (rand()) /((float) (RAND_MAX/(maxPosition*2)));
		snow[i].position.z = minPosition + (float) (rand()) /((float) (RAND_MAX/(maxPosition*2)));
		snow[i].firstPosition.x = snow[i].position.x;
		snow[i].firstPosition.y = snow[i].position.y;
		snow[i].firstPosition.z = snow[i].position.z;
		snow[i].direction.x = minDirectionX + (float) (rand()) /((float) (RAND_MAX/(maxDirectionX*2)));
		snow[i].direction.y = minDirectionY + (float) (rand()) /((float) (RAND_MAX/(maxDirectionY*2)));
		snow[i].direction.z = -1.0;
		snow[i].speed = (float) (rand()) /((float) (RAND_MAX/(maxParticleSpeed)));
	}
}


void drawParticles(Skier * ski){
	//Only emit particles if on slope
	if (isAirborne(ski) == ON)
		return;

	if(PARTICLE_DEPTH_SORT == 0){
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GEQUAL, 0.5);
	}
	else{
		//Sort coordinates in reverse order of depth
		//printf("camera x:%f y:%f z:%f ", ski->camera.x, ski->camera.y, ski->camera.z);
		setCameraPosition(ski);
		cameraPosition.x = ski->camera.x;
		cameraPosition.y = ski->camera.y;
		cameraPosition.z = ski->camera.z;
		//printf(" camera x:%f y:%f z:%f\n", cameraPosition.x, cameraPosition.y, cameraPosition.z);

		qsort(ski->particles.snow, PARTICLE_COUNT, sizeof(Vector3f), compareCamDistance);

		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	if(lightScene == ON)
		setLighting(OFF);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, snowballTexture);

	glPushMatrix();
		//translate skier into positive y-axis
		glTranslatef(0,0.1,0);
		//translate skier on sine wave
		glTranslatef(ski->position.x, ski->position.y, ski->position.z);
		//rotate to normal vector of surface
		//orientates x axis of 
		glRotatef(ski->rotation.x,1,0,0);
		glRotatef(ski->rotation.z,0,0,1);
		//rotate to follow heading
		glRotatef(ski->rotation.y, 0, 1, 0);

		glPushMatrix();
			glTranslatef(0,0,-0.15);
			int i;
			//glPointSize(6.0);
			for(i = 0; i < PARTICLE_COUNT; i++){
					/*glBegin(GL_POINTS);
					glVertex3f(snow[i].position.x, snow[i].position.y, snow[i].position.z);
					glEnd();*/
					glPushMatrix();
						glTranslatef(ski->particles.snow[i].position.x, ski->particles.snow[i].position.y,
							ski->particles.snow[i].position.z);
						startBillboard();
							drawTexQuad(PARTICLE_SIZE);
						glPopMatrix();
					glPopMatrix();
			}
		glPopMatrix();
		
		if(lightScene == ON)
			setLighting(ON);

	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}

void drawPlayerTrees(Skier * ski){
	float playerX = ski->minSlope.x;
	float playerZ = ski->minSlope.z;

	glPushMatrix();
	glTranslatef(playerX,0.0,playerZ);
	drawTrees(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+xWaveTranslate,0.0,playerZ);
	drawTrees(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+-xWaveTranslate,0.0,playerZ);
	drawTrees(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX,0.0,zWaveTranslate+playerZ);
	drawTrees(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX,0.0,-zWaveTranslate+playerZ);
	drawTrees(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+xWaveTranslate,0.0,zWaveTranslate+playerZ);
	drawTrees(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+-xWaveTranslate,0.0,zWaveTranslate+playerZ);
	drawTrees(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+xWaveTranslate,0.0,-zWaveTranslate+playerZ);
	drawTrees(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+-xWaveTranslate,0.0,-zWaveTranslate+playerZ);
	drawTrees(ski);
	glPopMatrix();


}