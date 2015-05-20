/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_skier.c
 * Description		: Skier functions
 -------------------------------------------------------------------------------*/
#include "ski_skier.h"

Toggle isAirborne(Skier * ski){
	float slopeY = getSine(ski->position.x, ski->position.z);
	if(ski->position.y > slopeY)
		return ON;
	return OFF;
}

void moveSkier(Skier * ski, float dTime, Toggle forward, Toggle back, Toggle left, Toggle right){
	/*Move ski player*/
	float rotateV = 120;

	//Get last frames y-value
	float slopeY = getSine(ski->position.x, ski->position.z);
	Toggle airborne = OFF;
	if(ski->position.y > slopeY)
		airborne = ON;

	if (left == ON){
		//Rotate on y axis
		ski->rotation.y += rotateV * dTime;

		//prevent buffer overflow by reducing ski->rotation.y to -180 to 180
		if(ski->rotation.y > 180)
			ski->rotation.y-=360;
		else if (ski->rotation.y < -180)
			ski->rotation.y+=360;

		//printf("ski->rotation.y:%f\n",ski->rotation.y);
	}
	if (right == ON){
		ski->rotation.y -= rotateV * dTime;

		//prevent buffer overflow by reducing ski->rotation.y to -180 to 180
		if(ski->rotation.y > 180)
			ski->rotation.y-=360;
		else if (ski->rotation.y < -180)
			ski->rotation.y+=360;

		//printf("ski->rotation.y:%f\n",ski->rotation.y);
	}
	if (forward == ON && airborne == OFF){

		/*ski->velocity.player += ACCEL_INCREMENT * dTime;
		if(ski->velocity.player > MAX_SPEED){
			ski->velocity.player = MAX_SPEED;
		}*/
		ski->acceleration.player += ACCEL_INCREMENT;
		ski->velocity.player = ski->acceleration.player * dTime;
		if(ski->velocity.player > MAX_SPEED){
			ski->velocity.player = MAX_SPEED;
			ski->acceleration.player -= ACCEL_INCREMENT;
		}
	}

	else if (back == ON && airborne == OFF){
		ski->acceleration.player -= ACCEL_INCREMENT;
		ski->velocity.player = ski->acceleration.player * dTime;
		if(ski->velocity.player <= 0){
			ski->velocity.player = 0;
			ski->acceleration.player = 0;
		}
	}
	
	float xAngle, zAngle;
	//if the skier is above the slope but not actually on it
	if(airborne == ON){
		float drag = 0.5 * C_DRAG * ski->velocity.slope.y * ski->velocity.slope.y;
		//ski->acceleration.y = GRAVITY;
		//then it should fall due to gravity
		//velocityByGravity += GRAVITY;
		ski->velocity.slope.y += dTime * (GRAVITY + drag);
		ski->lastPosition = ski->position;
		ski->position.y += dTime * (ski->velocity.slope.y + dTime * GRAVITY * 0.5);
		ski->position.x += dTime * ski->velocity.player;
		ski->position.z += dTime * ski->velocity.player;
		ski->airTime += dTime;
	}
	else{
		if(ski->lastPosition.y > slopeY){ //+0.5??
			ski->velocity.slope.y = 0;
			ski->acceleration.slope.y = GRAVITY;
		}
			

		float xN = getDxySine(ski->position.x); 
		float yN = 1;
		float zN = getDzySine(ski->position.z);

		float drag = 0.5 * C_DRAG * ski->velocity.slope.z * ski->velocity.slope.z;

		//calculate z-axis acceleration on a Frictionless Slope
		zAngle = atan2(zN,yN);// 179.99 / 360.0 * 2.0 * M_PI;
		ski->acceleration.slope.z = GRAVITY * sinf(zAngle) + drag;
		ski->velocity.slope.z += ski->acceleration.slope.z *dTime;
		//printf("ski->velocity.slope.z%f sinf(zAngle)%f \n", ski->velocity.slope.z, sinf(zAngle));

		//calculate z-axis friction's effect on velocity
		ski->velocity.slope.z *= pow(1 - FRICTION, dTime);
		//printf("with friction ski->velocity.slope.z%f\n", ski->velocity.slope.z);

		drag = 0.5 * C_DRAG * ski->velocity.slope.x * ski->velocity.slope.x;
		//do the same for the x axis
		xAngle = atan2(xN,yN);// 179.99 / 360.0 * 2.0 * M_PI;
		ski->acceleration.slope.x = GRAVITY * sinf(xAngle) + drag;
		ski->velocity.slope.x += ski->acceleration.slope.x *dTime;
	//	printf("ski->velocity.slope.x%f sinf(xAngle)%f \n", ski->velocity.slope.x, sinf(xAngle));

		ski->velocity.slope.x *= pow(1 - FRICTION, dTime);
		//printf("with friction ski->velocity.slope.x%f\n", ski->velocity.slope.x);
	
		//ski->position.x += dTime * ski->velocity.player * sinf(ski->rotation.y*M_PI/180);
		//Integrate player and slope force separately
		ski->lastPosition = ski->position;
		ski->position.x += dTime * (ski->velocity.player +dTime * ski->acceleration.player * 0.5) * sinf(ski->rotation.y*M_PI/180);
		ski->position.x += dTime * (ski->velocity.slope.x + dTime * ski->acceleration.slope.x * 0.5);
		ski->position.z += dTime * (ski->velocity.player +dTime * ski->acceleration.player * 0.5) * cosf(ski->rotation.y*M_PI/180);
		ski->position.z += dTime * (ski->velocity.slope.z + dTime * ski->acceleration.slope.z * 0.5);
	/*}
	if(airborne == OFF){*/
		//calculate the y velocity

		ski->position.y = getSine(ski->position.x, ski->position.z);
		/*Vector3f vectorCrossProduct = {ski->lastPosition.y*ski->position.z - ski->lastPosition.z*ski->position.y,
								ski->lastPosition.z*ski->position.x - ski->lastPosition.x*ski->position.z,
								ski->lastPosition.x*ski->position.y - ski->lastPosition.y*ski->position.x};*/
		//loat angle = cosf(xAngle);
		//ski->acceleration.slope.y = GRAVITY * angle + drag;
		//ski->velocity.slope.y += ski->acceleration.slope.y *dTime;
		
		//ski->velocity.slope.y += GRAVITY * dTime * -sinf(xAngle); //* ski->acceleration.slope.y;


		//ski->position.y += dTime * (ski->velocity.slope.y + dTime * ski->acceleration.slope.y * 0.5);



		//float yForwardVector = -sinf(xAngle) * dTime;

		//ski->position.y += ski->position.y *  yForwardVector;
//		ski->position.y += ski->velocity.slope.y;
		//float yForwardVector = -sinf(xAngle);

		/*ski->acceleration.slope.y = +GRAVITY * cosf(zAngle) * cosf(xAngle)* yForwardVector;;
		
		ski->velocity.slope.y +=  ski->acceleration.slope.y*dTime;*/

		//printf("accY %f velY %f angle %f cosf(angle) %f sinf(angle) %f\n", ski->acceleration.slope.y,ski->velocity.slope.y, xAngle, cosf(xAngle), sinf(xAngle));

		//ski->position.y += dTime * (ski->velocity.slope.y + dTime * ski->acceleration.slope.y * 0.5);


		//printf("ski->velocity.slope.y%f sinf(yAngle)%f", ski->velocity.slope.y, sinf(zAngle));
		//ski->position.y += dTime * ski->velocity.slope.y;
		//printf("ypositon %f yForwardVector%f \n", ski->position.y, yForwardVector);
		//ski->position.y = yForwardVector +  dTime * (ski->velocity.slope.y + dTime * ski->acceleration.slope.y * 0.5);

		orientateSkier(ski);
	}
}


//glScale will also scale the normals which will effect lighting
//Therefore set cube vertices as 0.1 instead of 1
float cubeVerts[]={
	//Top
	0.1,0.1,-0.1,	-0.1,0.1,-0.1,	-0.1,0.1,0.1,	0.1,0.1,0.1,
	//Bottom
	0.1,-0.1,0.1,	-0.1,-0.1,0.1,	-0.1,-0.1,-0.1,	0.1,-0.1,-0.1,
	//Front
	0.1,0.1,0.1,	-0.1,0.1,0.1,	-0.1,-0.1,0.1,	0.1,-0.1,0.1,
	//Back
	0.1,-0.1,-0.1,	-0.1,-0.1,-0.1,	-0.1,0.1,-0.1,	0.1,0.1,-0.1,
	//Left
	-0.1,0.1,0.1,	-0.1,0.1,-0.1,	-0.1,-0.1,-0.1,	-0.1,-0.1,0.1,
	//Right
	0.1,0.1,-0.1,	0.1,0.1,0.1,	0.1,-0.1,0.1,	0.1,-0.1,-0.1};

float cubeNormals[]={
	//Top
	0.0,1.0,-0.0,	-0.0,1.0,-0.0,	-0.0,1.0,0.0,	0.0,1.0,0.0,
	//Bottom
	0.0,-1.0,0.0,	-0.0,-1.0,0.0,	-0.0,-1.0,-0.0,	0.0,-1.0,-0.0,
	//Front
	0.0,0.0,1.0,	-0.0,0.0,1.0,	-0.0,-0.0,1.0,	0.0,-0.0,1.0,
	//Back
	0.0,-0.0,-1.0,	-0.0,-0.0,-1.0,	-0.0,0.0,-1.0,	0.0,0.0,-1.0,
	//Left
	-1.0,0.0,0.0,	-1.0,0.0,-0.0,	-1.0,-0.0,-0.0,	-1.0,-0.0,0.0,
	//Right
	1.0,0.0,-0.0,	1.0,0.0,0.0,	1.0,-0.0,0.0,	1.0,-0.0,-0.0};

//draw a cube given vertices and normals
void drawCube(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//cube is in 3 axes, hence 3
	glVertexPointer(3,GL_FLOAT,0,cubeVerts);
	glNormalPointer(GL_FLOAT,0,cubeNormals);

	/*24 vertices to cube (actually 8 but there has to be 24 normal
	 vectors for proper lighting)*/
	glDrawArrays(GL_QUADS, 0, 24);
}

//Draw cube Normals
void drawCubeNormals(){
	int i;
	float decr=0.25;

	if(lightScene == ON)
		setLighting(OFF);
	//24 verts * 3 dimension values = 72
	for (i=0; i < 72; i+=3)	{			
		glBegin(GL_LINES);
		glVertex3f(cubeVerts[i],cubeVerts[i+1],cubeVerts[i+2]);
		glVertex3f(cubeVerts[i]+cubeNormals[i]*decr,
			cubeVerts[i+1]+cubeNormals[i+1]*decr,
			cubeVerts[i+2]+cubeNormals[i+2]*decr);
		glEnd();

#ifdef DEBUG_NORM
		printf("x:%f y:%f z:%f xN:%f yN:%f zN:%f\n",
			gridVerts[i],gridVerts[i+1],gridVerts[i+2],
			gridNormals[i],gridNormals[i+1],gridNormals[i+2]);
#endif
	}
	if(lightScene == ON)
		setLighting(ON);
}

Skier ski1 = {
	{SKI1_X, SKI1_Y, SKI1_Z}, //position
	{0.0, 0.0, 0.0}, //last position
	{0.0, 0.0, 0.0},//rotation, heading of skier, orientation of X and Z on slope
	{{0.0, 0.0, 0.0},0.0,0.0}, //velocity
	{{0.0, 0.0, 0.0},0.0},//acceleration
	{0.0, OFF},//collisions
	SKI1_RADIUS,//radius
	{0.0,},//particle elapsed time
	{0,0,0},//camera position
	0.0,//airtime
	{0.0, 0.0, 0.0},//infinite terrain
	{0.0, 0.0, 0.0}
};

Skier ski2 = {
	{SKI2_X, SKI2_Y, SKI2_Z}, //position
	{0.0, 0.0, 0.0}, //last position
	{0.0, 0.0, 0.0},//rotation, heading of skier, orientation of X and Z on slope
	{{0.0, 0.0, 0.0},0.0,0.0}, //velocity
	{{0.0, 0.0, 0.0},0.0},//acceleration
	{0.0, OFF},//collisions
	SKI2_RADIUS,//radius
	{0.0,},//particle elapsed time
	{0,0,0},//camera position
	0.0,//airtime
	{0.0, 0.0, 0.0}, //infinite terrain
	{0.0, 0.0, 0.0}
};

void orientateSkier(Skier * ski){
	//determine dy/dx and dz/dx for orientation
	//values do not need to be normalised
	float xN = getDxySine(ski->position.x); 
	float yN = 1;
	float zN = -getDzySine(ski->position.z);

	//determine angle to orientate cube
	//both xN and yN and zN are required to determine quadrant based on values signs
	//atan2 computes arctangent of y/x and y/z in radians, hence the conversion
	ski->rotation.z = atan2(xN,yN)*180/M_PI;
	ski->rotation.x = atan2(zN,yN)*180/M_PI;
}

void drawSkierHierarchy(){
//assume have translated to skier position
	glPushMatrix();
		glEnable(GL_NORMALIZE);

		//render torso
		glPushMatrix();
			glTranslatef(0,0.08,0);
			glRotatef(15,1,0,0);
			glScalef(0.3, 0.5, 0.14);
			drawCube();
		glPopMatrix();

		//render neck
		glPushMatrix();
			glRotatef(12,1,0,0);
			glTranslatef(0,0.136,-0.015);
			glScalef(0.09, 0.08, 0.09);
			drawCube();
		glPopMatrix();

		//render head
		glPushMatrix();
			glTranslatef(0,0.172,0.02);
			glutSolidSphere(0.03, 16,16);
		glPopMatrix();

		//render left shoulder
		glPushMatrix();
			glTranslatef(-0.034,0.12,0.01);
			glRotatef(-3,0,0,1);
			glScalef(0.05, 0.1, 0.1);
			drawCube();
		glPopMatrix();

		//render left upper arm
		glPushMatrix();
			glTranslatef(-0.05,0.11,0.01);
			glRotatef(-30,1,0,0);
			glRotatef(-9,0,0,1);
			glScalef(0.1, 0.2, 0.1);
			drawCube();
		glPopMatrix();

		//render left lower arm
		glPushMatrix();
			glTranslatef(-0.05,0.08,0.036);
			glRotatef(-49,1,0,0);
			glScalef(0.1, 0.2, 0.1);
			drawCube();
		glPopMatrix();

		//render left ski pole
		glPushMatrix();
			glTranslatef(-0.055,0.004,0.023);
			glRotatef(28,1,0,0);
			glRotatef(-4,0,0,1);
			glScalef(0.028, 0.8, 0.028);
			drawCube();
		glPopMatrix();

		glPushMatrix();
			//move leg slightly to the left
			glTranslatef(-0.006,0,0);

			//render left upper leg
			glPushMatrix();
				glTranslatef(-0.018,0.0135,0.007);
				glRotatef(-75,1,0,0);
				glScalef(0.14, 0.34, 0.15);
				drawCube();
			glPopMatrix();

			//render left lower leg
			glPushMatrix();
				glTranslatef(-0.018,-0.03,0.009);
				glRotatef(28,1,0,0);
				glScalef(0.14, 0.34, 0.15);
				drawCube();
			glPopMatrix();

			//render left foot
			glPushMatrix();
				glTranslatef(-0.018,-0.06,0.0046);
				glScalef(0.1, 0.1, 0.25);
				drawCube();
			glPopMatrix();

			//render left ski 
			glPushMatrix();
				glTranslatef(-0.018,-0.069,0.009);
				glScalef(0.14, 0.01, 1);
				drawCube();
			glPopMatrix();

		glPopMatrix();

		//repeat for right side
		//render right shoulder
		glPushMatrix();
			glTranslatef(0.034,0.12,0.01);
			glRotatef(3,0,0,1);
			glScalef(0.05, 0.1, 0.1);
			drawCube();
		glPopMatrix();

		//render right upper arm
		glPushMatrix();
			glTranslatef(0.05,0.11,0.01);
			glRotatef(-30,1,0,0);
			glRotatef(9,0,0,1);
			glScalef(0.1, 0.2, 0.1);
			drawCube();
		glPopMatrix();

		//render right lower arm
		glPushMatrix();
			glTranslatef(0.05,0.08,0.036);
			glRotatef(-49,1,0,0);
			glScalef(0.1, 0.2, 0.1);
			drawCube();
		glPopMatrix();

		//render right ski pole
		glPushMatrix();
			glTranslatef(0.055,0.004,0.023);
			glRotatef(28,1,0,0);
			glRotatef(4,0,0,1);
			glScalef(0.028, 0.8, 0.028);
			drawCube();
		glPopMatrix();

		glPushMatrix();
			//move leg slightly to the right
			glTranslatef(0.006,0,0);

			//render right upper leg
			glPushMatrix();
				glTranslatef(0.018,0.0135,0.007);
				glRotatef(-75,1,0,0);
				glScalef(0.14, 0.34, 0.15);
				drawCube();
			glPopMatrix();

			//render right lower leg
			glPushMatrix();
				glTranslatef(0.018,-0.03,0.009);
				glRotatef(28,1,0,0);
				glScalef(0.14, 0.34, 0.15);
				drawCube();
			glPopMatrix();

			//render right foot
			glPushMatrix();
				glTranslatef(0.018,-0.06,0.0046);
				glScalef(0.1, 0.1, 0.25);
				drawCube();
			glPopMatrix();

			//render right ski 
			glPushMatrix();
				glTranslatef(0.018,-0.069,0.009);
				glScalef(0.14, 0.01, 1);
				drawCube();
			glPopMatrix();

		glPopMatrix();

		glDisable(GL_NORMALIZE);
	glPopMatrix();
}

//translate and draw the skier
void drawSkier(Skier * ski){
	glEnable(GL_COLOR_MATERIAL);
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

		//draw axis bigger than skier so it can be seen when wire frame is off
		glPushMatrix();
			glScalef(0.3, 0.3, 0.3);
			drawAxis();
		glPopMatrix();

		//for normal lines and when lighting is disabled
		glColor3f(1.0,0.5,0);
		/*Bi-normals and tangents are the same as normals for cube
		(they just refer to different vertices)*/
		if (viewNormal == ON || viewBinormal == ON || viewTangent == ON)
			drawCubeNormals();

		float orange[] = {1.0,0.5,0.0};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, orange);
		
		//drawCube();
		drawSkierHierarchy();
		//reset colour back to white*/
		glColor3f(1,1,1);

		glDisable(GL_COLOR_MATERIAL);

	glPopMatrix();
}

