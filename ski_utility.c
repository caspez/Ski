/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_utility.c
 * Description		: Utility Functions
 -------------------------------------------------------------------------------*/
#include "ski_utility.h"

GLuint loadTexture(const char *filename, Toggle toggle){
	GLuint texture = SOIL_load_OGL_texture(filename, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
	if (!texture)
		return 0;

	glBindTexture(GL_TEXTURE_2D, texture);
	if(toggle == ON){
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	}
		
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

/*Generate random seed*/
void initRandomSeed(){
	struct timeval timeValue;
	gettimeofday(&timeValue, NULL);
	srand(timeValue.tv_usec);
}

int checkForCollision(Skier * ski){
	/*check if skier collided with tree*/
	float combinedRadius = trees.radius + ski->radius;

	//Get relative coordinates 
	float x = ski->position.x - ski->minSlope.x;
	float y = ski->position.y;
	float z = ski->position.z - ski->minSlope.z;

	//printf("x %f y %f z %f \n",x,y,z);
	//find the distance between two points
	//(middle of skier and middle of each tree)
	Toggle frameCollision = OFF;
	
	int i = 0;
	int tree;
	for(tree = 0; tree < trees.count; tree++){
		frameCollision = OFF;
		float distance = sqrt((x-trees.coordinates[i].x)*(x-trees.coordinates[i].x)
						+ (y-trees.coordinates[i].y)*(y-trees.coordinates[i].y)
						+ (z-trees.coordinates[i].z)*(z-trees.coordinates[i].z));
		i+=3;
		if(distance < combinedRadius){
			frameCollision = ON;
			break;
		}
	}
	if(frameCollision == ON){
		/*So duplicate collisions are not counted*/
		if(ski->collision.previous == OFF){
			ski->collision.count++;
			ski->collision.previous = ON;
			if(ski->collision.count >= COLLISIONS_TO_LOSE){
				return 1;
			}
			return 0;
		}
	}
	else{
		ski->collision.previous = OFF;
	}
	return -1;
}


int checkForCollisionWrap(Skier * ski){
	int collisionCount = 0;

	float playerX = ski->minSlope.x;
	float playerZ = ski->minSlope.z;

	glPushMatrix();
	glTranslatef(playerX,0.0,playerZ);
	collisionCount += checkForCollision(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+xWaveTranslate,0.0,playerZ);
	collisionCount += checkForCollision(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+-xWaveTranslate,0.0,playerZ);
	collisionCount += checkForCollision(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX,0.0,zWaveTranslate+playerZ);
	collisionCount += checkForCollision(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX,0.0,-zWaveTranslate+playerZ);
	collisionCount += checkForCollision(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+xWaveTranslate,0.0,zWaveTranslate+playerZ);
	collisionCount += checkForCollision(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+-xWaveTranslate,0.0,zWaveTranslate+playerZ);
	collisionCount += checkForCollision(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+xWaveTranslate,0.0,-zWaveTranslate+playerZ);
	collisionCount += checkForCollision(ski);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+-xWaveTranslate,0.0,-zWaveTranslate+playerZ);
	collisionCount += checkForCollision(ski);
	glPopMatrix();

	if(ski->collision.count >= COLLISIONS_TO_LOSE){
		return 1;
	}
	return 0;
}