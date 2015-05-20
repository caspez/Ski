/*-------------------------------------------------------------------------------
 * Ski
 * Name				: Caspersz, Joshua
 * File				: ski_main.h
 * Description		: Main file header
 -------------------------------------------------------------------------------*/

//System header files
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>

//OpenGL header files
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "./lib/SOIL.h"

//Program header files
#include "ski_skier.h"
#include "ski_display.h"
#include "ski_input.h"
#include "ski_slope.h"
#include "ski_view.h"
#include "ski_utility.h"
#include "ski_billboards.h"

#ifndef SKI_MAIN_H
#define SKI_MAIN_H

//Make sure to make clean after changing these

#define GAME_TIME 30
#define COLLISIONS_TO_LOSE 5
//Depth sorting trees can cause game to be more difficult
#define TREE_DEPTH_SORT 1 //set to 0 to disable
#define TREE_COUNT 5 //number of trees to generate per slope square
#define TREE_RADIUS 0.5 //decrease to reduce difficulty

#define DRAW_PARTICLES 1
#define PARTICLE_DEPTH_SORT 1 //Disable only to increase performance
#define PARTICLE_COUNT 100 //snow ball particles
#define PARTICLE_REGEN 2 //seconds to next particle generation
#define PARTICLE_MAX_POSITION 0.1
#define PARTICLE_MAX_DIRECTION_X 0.4
#define PARTICLE_MAX_DIRECTION_Y 0.4
#define PARTICLE_MAX_SPEED 3.0
#define PARTICLE_SIZE 0.05

//PHYSICS CONSTANTSddddddddddddddddddddddddddd
#define ACCEL_INCREMENT 0.6 //player acceleration
#define MAX_SPEED 2.9
#define FRICTION 0.8
#define C_DRAG 0.1
#define GRAVITY -9.8

//determines number of triangles per wave
#define SLOPE_QUALITY 10.0
//determines number of rows and columns of triangles
#define ROWS 50
#define COLS 50
#define TEXTURE_TILING 0.5
#define X_WAVE_INCREMENT 4.0
#define Z_WAVE_INCREMENT 4.94

//Default Window Dimensions
#define DEFAULT_HEIGHT 700
#define DEFAULT_WIDTH 700

//Default Camera values
#define CAMERA_X 30.0
#define CAMERA_Y 45.0
#define CAMERA_ZOOM -0.6

//Default player positions
#define SKI1_X 0.0
#define SKI1_Y 7.0
#define SKI1_Z 0.0
#define SKI2_X 0.5
#define SKI2_Y 7.0
#define SKI2_Z 0.0
#define SKI1_RADIUS 0.5 //decrease to reduce difficulty
#define SKI2_RADIUS 0.5 //decrease to reduce difficulty

//define toggle data type
typedef enum {OFF = 0, ON} Toggle;

typedef struct{
	float x, y;
}Vector2f;

typedef struct{
	float x, y, z;
}Vector3f;

typedef struct{
	Vector2f rotation;
	float zoom;
}Camera;

typedef struct{
	GLuint texture;
	float radius;
	int count;
	int coordCount;
	Vector3f * coordinates;
}Trees;

typedef struct{
	Vector3f position;
	Vector3f firstPosition;
	Vector3f direction;
	float speed;
}Particle;

typedef struct{
	Vector3f position;
	Vector3f lastPosition;
	Vector3f rotation;
	struct{
		Vector3f slope;
		float player;
		float gravity;
	}velocity;
	struct{
		Vector3f slope;
		float player;
	}acceleration;
	struct{
		int count;
		Toggle previous;
	}collision;
	float radius;
	struct{
		float elapsedTime;
		Particle snow[PARTICLE_COUNT];
	}particles;
	Vector3f camera;
	float airTime;
	Vector3f minSlope;
	Vector3f maxSlope;
}Skier;

//debugging toggles
#define DEBUG_INPUT
#undef DEBUG_INPUT
#define DEBUG_NORM
#undef DEBUG_NORM
#define DEBUG_VERT
#undef DEBUG_VERT
#define DEBUG_ORIENTATION
#undef DEBUG_ORIENTATION

void exitProgram();

#endif
