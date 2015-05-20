/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_slope.c
 * Description		: Slope functions
 -------------------------------------------------------------------------------*/
#include "ski_slope.h"

float xWaveLen= 1.6, zWaveLen= 1.8;
float xAmp =0.3, zAmp =-0.4;
float xKP = 0.8, zKP = 1.45;
/*This was put in a function so it can also be used to determine cube placement and
orientation. Hence the preceding variables.*/
float getSine(float x, float z){
	float xK = xKP * M_PI/xWaveLen;
	float zK = zKP * M_PI/zWaveLen;
	return xAmp * sinf(xK*x) + zAmp * sinf(zK*z);
}

float getDxySine(float x){
	float xK = xKP * M_PI/xWaveLen;
	return xK * xAmp * cosf(xK*x);
}

float getDzySine(float z){
	float zK = zKP * M_PI/zWaveLen;
	return zK * zAmp * cosf(zK*z);
}



//number of vertices per square (2 triangles)
int triVert = 6;
//counter for slope vertices and normals
int slopeVCount, slopeNCount;
int slopeTCount;
//arrays of slope vertices and normals
float * slopeVerts, * slopeNormals;
float * slopeTextureCs;

//allocate memory for the arrays and insert normal & vertices values
void generateSlope(){
	//size refers to size of triangle side
	//(Distance from -1 to 1 is 2 / #rows)
	float size = 2/SLOPE_QUALITY;
	//actual rows and columns relates to the number of waves
	float actualRows = ROWS/10;
	float actualCols = COLS/10;
	float x1=0, x2=0, y1=0, y2=0, y3=0, y4=0, z1=0, z2=0;
	//Partial derivative values
	float dX1=0, dX2=0;
	float dZ1=0, dZ2=0;
	//Normal values
	float yN1=1, yN2=1, yN3=1, yN4=1;
	float xN1=0, xN2=0, zN1=0, zN2=0;
	float len1=0, len2=0, len3=0, len4=0;

	//3 values (x,y,z) for each 6 vertices used to make 1 square from two triangles
	slopeVerts = (float*) malloc(SLOPE_QUALITY*ROWS*COLS*triVert*3*sizeof(float));
	if(slopeVerts == NULL){
		printf("Failed to allocate memory for slope verticies\n");
		exit(EXIT_FAILURE);
	}
	slopeVCount = 0;

	slopeNormals = (float*) malloc(SLOPE_QUALITY*ROWS*COLS*triVert*3*sizeof(float));
	if(slopeNormals == NULL){
		printf("Failed to allocate memory for slope normals\n");
		exit(EXIT_FAILURE);
	}
	slopeNCount = 0;


	slopeTextureCs = (float*) malloc(SLOPE_QUALITY*ROWS*COLS*triVert*2*sizeof(float));
	if(slopeTextureCs == NULL){
		printf("Failed to allocate memory for slope texture coordinates\n");
		exit(EXIT_FAILURE);
	}
	slopeTCount = 0;

	float textureRows = TEXTURE_TILING/actualRows;
	float textureCols = TEXTURE_TILING/actualCols;

	float currentColTex = 0;
	//from z axis -1 to 1-size
	//(last square will reach z=1)
	for (z1=0; z1 < actualCols-size; z1+=size){
		float currentRowTex = 0;
		//from x axis -1 to 1-size
		//(again last square will reach x=1)
		for (x1 = 0; x1 < actualRows-size; x1+=size){
			x2 = x1 + size;
			z2 = z1 + size;

			//Calculate y vertex values
			y1 = getSine(x1,z1);
			y2 = getSine(x2,z1);
			y3 = getSine(x1,z2);
			y4 = getSine(x2,z2);

			//Insert triangle vectors into array
			float currentVerts[] ={
				x1,y1,z1,
				x1,y3,z2,
				x2,y4,z2,
				x1,y1,z1,
				x2,y2,z1,
				x2,y4,z2};
			int i;
			int count = sizeof(currentVerts)/sizeof(float);
			for( i = 0; i<count; i++){
				slopeVerts[slopeVCount+i] = currentVerts[i];
				//	if(slopeVerts[vcount+i]>1 || slopeVerts[vcount+i] < -1)
				//			printf("error:%f",vcount+i);
			}
			
			//Calculate Partial derivative values
			dX1 = getDxySine(x1);
			dX2 = getDxySine(x2);
			dZ1 = getDzySine(z1);
			dZ2 = getDzySine(z2);

			//Determine normal values
			xN1 = -dX1;
			xN2 = -dX2;
			zN1 = -dZ1;
			zN2 = -dZ2;
			//yN = 1

			//Get length of vectors
			//since yN will always equal 1, 1^2=1
			len1 = sqrt(xN1*xN1 + 1 + zN1*zN1);
			len2 = sqrt(xN1*xN1 + 1 + zN2*zN2); 
			len3 = sqrt(xN2*xN2 + 1 + zN2*zN2);
			len4 = sqrt(xN2*xN2 + 1 + zN1*zN1);
			
			//Normalise vector normals and insert values into array
			/*float currentNormals[] ={
				xN1, yN1, zN1,
				xN1, yN3, zN2,
				xN2, yN4, zN2,
				xN1, yN1, zN1,
				xN2, yN2, zN1,
				xN2, yN4, zN2};*/
			float currentNormals[] ={
				xN1 / len1, yN1 / len1, zN1 / len1,
				xN1 / len2, yN3 / len2, zN2 / len2,
				xN2 / len3, yN4 / len3, zN2 / len3,
				xN1 / len1, yN1 / len1, zN1 / len1,
				xN2 / len4, yN2 / len4, zN1 / len4,
				xN2 / len3, yN4 / len3, zN2 / len3};
			count = sizeof(currentNormals)/sizeof(float);
			for( i = 0; i<count; i++){
				slopeNormals[slopeNCount+i] = currentNormals[i];
				//	if(slopeVerts[vcount+i]>1 || slopeVerts[vcount+i] < -1)
				//			printf("error:%f",vcount+i);
			}

			//Insert texture coordinates into array
			/*float currentTexs[] ={
				-xN1,-zN1,
				-xN1,-zN2,
				-xN2,-zN2,
				-xN1,-zN1,
				-xN2,-zN1,
				-xN2,-zN2};*/
			/*float currentTexs[] ={
				0,0,
				0,1,
				1,1,
				0,0,
				1,0,
				1,1};*/
			float textMaxX = currentRowTex+textureRows;
			float textMaxY = currentColTex+textureCols;
			float currentTexs[] ={
				currentRowTex,currentColTex,
				currentRowTex,textMaxY,
				textMaxX,textMaxY,
				currentRowTex,currentColTex,
				textMaxX,currentColTex,
				textMaxX,textMaxY};
			currentRowTex+=textureRows;
			
			int textureCount = sizeof(currentTexs)/sizeof(float);
			for( i = 0; i<textureCount; i++){
				slopeTextureCs[slopeTCount+i] = currentTexs[i]*2;
			}


#ifdef DEBUG_NORM
			//len should = 1 and normal.tangent should equal 0
			float tanD1 = xN1*yN1 + yN1*(-xN1); 
			float tanD2 = xN2*yN2 + yN2*(-xN2); 
			printf("x1:%f y1:%f yD1:%f len1:%f xN1:%f yN1:%f tanDot:%f\n"
					,x1,y1,yD1,len1,xN1,yN1,tanD1);
			printf("x2:%f y2:%f yD2:%f len2:%f xN2:%f yN2:%f tanDot:%f\n"
					,x2,y2,yD2,len2,xN2,yN2,tanD2);
#endif		
#ifdef DEBUG_VERT
			printf("\n\nVerts: ");
			for (i=0; i<count; i++)
				printf("%f ",currentVerts[i]);
			printf("\nVertsCopy: ");
			for (i=0; i<count; i++)
				printf("%f ",slopeVerts[slopeVCount+i]);
#endif
			//Increase vertex and normal count
			//(Needs to be done after debugging for correct debug values)
			slopeVCount += count;
			slopeNCount += count;
			slopeTCount += textureCount;
		}
		currentColTex+=textureCols;
	}
}

float guideDecrement=0.3;
//Normal = (-dy/dx,1,-dy/dz)
void drawSlopeNormals(){
	int i;
	for (i=0; i < slopeVCount; i+=3){
		glBegin(GL_LINES);
			glVertex3f(slopeVerts[i],slopeVerts[i+1],slopeVerts[i+2]);
			//Use pre calculatedwd normals
			glVertex3f(slopeVerts[i]+slopeNormals[i]*guideDecrement,
					slopeVerts[i+1]+slopeNormals[i+1]*guideDecrement,
					slopeVerts[i+2]+slopeNormals[i+2]*guideDecrement);
		glEnd();
#ifdef DEBUG_NORM
		printf("x:%f y:%f z:%f xN:%f yN:%f zN:%f\n",
			slopeVerts[i],slopeVerts[i+1],slopeVerts[i+2],
			slopeNormals[i],slopeNormals[i+1],slopeNormals[i+2]);
#endif
	}
}

//Binormal = (0,1,dy/dz)
void drawSlopeBinormals(){
	int i;
	for (i=0; i < slopeVCount; i+=3){
		glBegin(GL_LINES);
			glVertex3f(slopeVerts[i],slopeVerts[i+1],slopeVerts[i+2]);
			//Use pre calculated normals values to determine bi-normals
			//normal=(-dy/dx,1,-dy/dz), binormal = (0,dy/dz,1)
			glVertex3f(slopeVerts[i],
				//use minus to cancel out - in -dy/dz
				slopeVerts[i+1] - slopeNormals[i+2]*guideDecrement,
				slopeVerts[i+2] + slopeNormals[i+1]*guideDecrement);
		glEnd();
	}
}

//Tangent = (dy/dx,1,0)
void drawSlopeTangents(){
	int i;
	for (i=0; i < slopeVCount; i+=3){
		glBegin(GL_LINES);
			glVertex3f(slopeVerts[i],slopeVerts[i+1],slopeVerts[i+2]);
			//Use pre calculated normals values to determine tangents
			//Normal=(-dy/dx,1,-dy/dz), tangent = (1,dy/dx,0)
			glVertex3f(slopeVerts[i]+slopeNormals[i+1]*guideDecrement,
					//use minus to cancel out - in -dy/dx
					slopeVerts[i+1]-slopeNormals[i]*guideDecrement,
					slopeVerts[i+2]);
		glEnd();
	}
}

//Draw guide lines
void drawSlopeGuideLines(){
	if(lightScene == ON)
		setLighting(OFF);
	if (viewNormal == ON){
		glColor3f(1,1,0);
		drawSlopeNormals();
	}
	if (viewBinormal == ON){
		glColor3f(1,0,0.5);
		drawSlopeBinormals();
	}
	if (viewTangent == ON){
		glColor3f(0.5,0.5,1);
		drawSlopeTangents();
	}
	if(lightScene == ON)
		setLighting(ON);
}

GLuint snowTexture;
void drawSlope(){
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, snowTexture);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3,GL_FLOAT,0,slopeVerts);
	glNormalPointer(GL_FLOAT,0,slopeNormals);
	glTexCoordPointer(2,GL_FLOAT,0,slopeTextureCs);
	
	glColor3f(1,1,1);//for wireframe*/
	float white[] = {1.0,1.0,1.0};
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, white);

	glDrawArrays(GL_TRIANGLES, 0, SLOPE_QUALITY*ROWS*COLS*triVert);
	
	drawSlopeGuideLines();

	glDisable(GL_COLOR_MATERIAL);
	
}

void freeSlope(){
	free(slopeVerts);
	free(slopeNormals);
	free(slopeTextureCs);
}

void setSlopeBoundary(Skier * ski, float minX, float maxX, float minZ, float maxZ){
	ski->minSlope.x = minX;
	ski->minSlope.z = minZ;
	ski->maxSlope.x = maxX;
	ski->maxSlope.z = maxZ;
}

float xWaveTranslate = X_WAVE_INCREMENT;
float zWaveTranslate = Z_WAVE_INCREMENT;
void drawPlayerSlope(Skier * ski){
/*	printf("ski->position.x %f ski->position.z %f\n", ski->position.x,ski->position.z);
	printf("ski->minSlope.x %f ski->maxSlope.x %f \n", ski->minSlope.x, ski->maxSlope.x);
	printf("ski->minSlope.z %f ski->mazSlope.z %f \n", ski->minSlope.z, ski->maxSlope.z);*/
	if(ski->position.x < ski->minSlope.x){
		ski->minSlope.x -= xWaveTranslate;
		ski->maxSlope.x -= xWaveTranslate;
	}
	if(ski->position.z < ski->minSlope.z){
		ski->minSlope.z -= zWaveTranslate;
		ski->maxSlope.z -= zWaveTranslate;
	}

	if(ski->position.x > ski->maxSlope.x){
		ski->minSlope.x += xWaveTranslate;
		ski->maxSlope.x += xWaveTranslate;
	}

	if(ski->position.z > ski->maxSlope.z){
		ski->minSlope.z += zWaveTranslate;
		ski->maxSlope.z += zWaveTranslate;
	}

	
	float playerX = ski->minSlope.x;
	float playerZ = ski->minSlope.z;

	glPushMatrix();
	glTranslatef(playerX,0.0,playerZ);
	drawSlope();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+xWaveTranslate,0.0,playerZ);
	drawSlope();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+-xWaveTranslate,0.0,playerZ);
	drawSlope();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX,0.0,zWaveTranslate+playerZ);
	drawSlope();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX,0.0,-zWaveTranslate+playerZ);
	drawSlope();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+xWaveTranslate,0.0,zWaveTranslate+playerZ);
	drawSlope();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+-xWaveTranslate,0.0,zWaveTranslate+playerZ);
	drawSlope();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+xWaveTranslate,0.0,-zWaveTranslate+playerZ);
	drawSlope();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(playerX+-xWaveTranslate,0.0,-zWaveTranslate+playerZ);
	drawSlope();
	glPopMatrix();


}