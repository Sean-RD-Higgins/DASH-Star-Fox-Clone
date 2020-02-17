/*Programmer: Sean Higgins 
*/
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//includes
#include "include\structs.h"
#include "include\ship.h"
#include "include\fire.h"
#include "include\enemy.h"
#include <iostream>
#include <stdlib.h>
#include <math.h>

//global
class Arwing;
class Fire;
extern float screenWidth;
extern Star star[25];
extern Arwing ship;
extern Arwing ship2;
extern Enemy enemy;
extern Fire fire;
extern PyramidCorners leftWingEnd, rightWingEnd, leftWing, rightWing, leftTopJet, leftBottomJet, rightTopJet, rightBottomJet, topBody, bottomBody;
extern PyramidCorners fireFront, fireBack;
extern PyramidCorners enemyBody, enemyJet;
extern Point cursor[2];
extern Point crosshair[2];
extern int KeyDown[256];
extern BoundBox crosshairBoundBox;
extern int maxStars;
extern double debug[10];
extern unsigned char key[2][6];
extern int bearLeftKey, bearRightKey, rollLeftKey, rollRightKey, boostKey, brakeKey;
extern double deltaGameTime, currentGameTime, prevGameTime;


//returns the direction in radians from point x1,y1 to x2,y2
float pointDirection(float x1,float y1,float x2,float y2);
// Determines the distance between x1,y1 and x2,y2
float pointDistance(float x1,float y1, float x2, float y2);
// returns the sign of the function, -1 or 1
int sign(float num);
//redetermined length for screen acceleration
float sizeIndex();
//easily return a new point
Point newPoint(float x, float y, float z);
// init all stars
void initStars();
// handle stars
void handleStars();
//returns a random float between 0 and 1
float randFloat();
//requests a fire shot be called
void fireRequest(Point from, Point to, char * team, int attack, float speed, float AoE);
//check if this hitbox collides with ANY hitbox
HitBox detectCollision(HitBox hb);
//check if two hitboxes collide
HitBox singleCollideCheck(HitBox a, HitBox b);
//check if two lines cross
bool singleLineCheck(float aLeft, float aRight, float bLeft, float bRight);
//request a fire be deflected
void requestFireDeflect(HitBox box, char * team);
#endif