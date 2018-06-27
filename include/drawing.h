/*Programmer: Sean Higgins 
*/
#ifndef DRAWING_H
#define DRAWING_H

#include "include\structs.h"
#include "include\fire.h"
#include "include\ship.h"
#include "include\enemy.h"
#include "include\imageloader.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

//externals
class Arwing;
class Fire;
class Enemy;
extern GLuint _tid_star; //The id of the texture
extern GLuint _tid_crosshair; //The texture id of the crosshair texture
extern GLuint _tid_outerCrosshair; //The texture id of the outer crosshair texture
extern GLuint _tid_white; //The texture id of the ship
extern GLuint _tid_blue; //the texture ID of a blue texture
extern GLuint _tid_black; //the texture ID of a blacktexture
extern GLuint _tid_red; //the texture ID of a red texture
extern GLuint _tid_yellow; //the texture ID of a yellow texture
extern float timer;
extern float x,y,z,l,w,h;

void drawSimplePyramid(float l, GLuint texId);
void drawSimpleCube(float l,GLuint texId);
void drawExtQuad(float x,float y,float z,float l,float w,float h,GLuint texId);
void drawCornersPyramid(PyramidCorners p, GLuint texId);
void drawCornersOpenPyramid(PyramidCorners p, GLuint texId);
void drawArwing();
void drawArwing2();
void drawEnemy();
void drawHP();
void drawHP2();
void initCamLight();
void drawScene();
void initDrawItems();

#endif