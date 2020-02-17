#include "include\structs.h"
#include "include\ship.h"
#include "include\functions.h"
#include "include\fire.h"
#include "include\enemy.h"

float screenWidth;
int maxStars = 25;
Star star[25] = {0,0,0,0,0,0};
Arwing ship;
Arwing ship2;
Enemy enemy;
Fire fire;
Point cursor[2];
Point crosshair[2];
int KeyDown[256];
BoundBox crosshairBoundBox = {-2.2f,2.2f, 1.8f,-1.6f, -5, -30};
double debug[10];
unsigned char key[2][6];
int bearLeftKey = 0;
int bearRightKey = 1;
int rollLeftKey = 2;
int rollRightKey = 3;
int boostKey = 4;
int brakeKey = 5;
double deltaGameTime;

//Models
PyramidCorners leftWing, rightWing, leftTopJet, leftBottomJet, rightTopJet, rightBottomJet, topBody, bottomBody;
PyramidCorners fireFront, fireBack;
PyramidCorners enemyBody, enemyJet;

//returns the direction in radians from point x1,y1 to x2,y2
float pointDirection(float x1,float y1,float x2,float y2)
{
	return atan2((x1 - x2),(y1 - y2));
}
// Determines the distance between x1,y1 and x2,y2
float pointDistance(float x1,float y1, float x2, float y2)
{
	if(x1 - x2 < 0.0001 && x1 - x2 > -0.0001)
	{
		x1 = 0;
		x2 = 0;
	}
	if(y1 - y2 < 0.0001 && y1 - y2 > -0.0001)
	{
		y1 = 0;
		y2 = 0;
	}
	float answer = abs(sqrt(pow(abs(x1 - x2),2) + pow(abs(y1 - y2),2)));
	if(answer == UINT_MAX || answer == NULL)
	{
		answer = 0;
	}
	return answer;
}
// returns the sign of the function, -1 or 1
int sign(float num)
{
	if(num > 0)
	{
		return 1;
	}
	else if(num < 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}
//redetermined length for screen acceleration
float sizeIndex()
{
	float num = (screenWidth / 640.0f);
	if(num == 0.0f)
	{
		num = 1;
	}
	return num;
}
//new point
Point newPoint(float x, float y, float z)
{
	Point p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}
//makes all stars initialize
void initStars()
{
	for(int i = 0; i < maxStars;i++)
	{
		star[i].z = 7;
	}
	handleStars();
}
//fixes all stars that are passed the screen and bring them closer to the screen
void handleStars()
{
	for(int i = 0;i < maxStars;i++)
	{
		if(star[i].z > 6)
		{
			star[i].x = crosshairBoundBox.Left + (crosshairBoundBox.Right - crosshairBoundBox.Left) * randFloat();
			star[i].y = crosshairBoundBox.Top + (crosshairBoundBox.Bottom - crosshairBoundBox.Top)  * randFloat();
			star[i].z = -10.0f + 15.0f * randFloat();
			star[i].l = 0.02f + 0.02f * randFloat();
			star[i].w = 0.02f + 0.02f * randFloat(); 
		}
		star[i].z += (float) (ship.railAccel * deltaGameTime);
	}
}
//return a random float
float randFloat()
{
	return (float) rand() / 32000.0f;
}
//Request a fire shot be made and handled from a specific point towards a specific point
void fireRequest(Point from, Point to, char * team, int attack, float speed, float AoE)
{
	Point direction;
	direction.x = - sin(pointDirection(from.x,from.z,to.x,to.z));
	direction.y = - cos(pointDirection(from.z,from.y,to.z,to.y));
	direction.z = cos(pointDirection(from.x,from.z,to.x,to.z));

	//perform checking
	fire.fireRequest(from, direction, team, attack, speed, AoE);
}
//check for collisions
HitBox detectCollision(HitBox hb)
{
	HitBox ret;

	//check ship first
	if(ship.HP > 0)
	{
		ret = singleCollideCheck(hb, ship.hitbox[0]);
		if(ret.team != "False") return ret;
		ret = singleCollideCheck(hb, ship.hitbox[1]);
		if(ret.team != "False") return ret;
	}
	if(ship2.HP > 0)
	{
		ret = singleCollideCheck(hb, ship2.hitbox[0]);
		if(ret.team != "False") return ret;
		ret = singleCollideCheck(hb, ship2.hitbox[1]);
		if(ret.team != "False") return ret;
	}

	//check enemies
	for(int i = 0;i < enemy.maxEnemy;i++)
	{
		if(enemy.en[i].HP > 0)
		{
			ret = singleCollideCheck(hb, enemy.en[i].hitbox);
			if(ret.team != "False") return ret;
		}
	}

	//check fire
	for(int i = 0;i < fire.maxFire;i++)
	{
		if(fire.shot[i].team != "Empty")
		{
			ret = singleCollideCheck(hb, fire.shot[i].hitbox);
			if(ret.team != "False") return ret;
		}
	}

	return ret;
}
//check for a single hitbox collision
HitBox singleCollideCheck(HitBox a, HitBox b)
{
	if(a.team != b.team)
	{
		if(singleLineCheck(a.box.FrontTopLeft.x, a.box.BackBottomRight.x, b.box.FrontTopLeft.x, b.box.BackBottomRight.x))
		{
			if(singleLineCheck(a.box.FrontTopLeft.y, a.box.BackBottomRight.y, b.box.FrontTopLeft.y, b.box.BackBottomRight.y))
			{
				if(singleLineCheck(a.box.FrontTopLeft.z, a.box.BackBottomRight.z, b.box.FrontTopLeft.z, b.box.BackBottomRight.z))
				{
					return b;
				}
			}
		}
	}
	HitBox hb;
	hb.team = "False";
	return hb;
}
//check if 2 lines collide with each other
bool singleLineCheck(float aLeft, float aRight, float bLeft, float bRight)
{
	// If box A is on the left...
	if(aLeft < bLeft)
	{
		// if the difference between a and b's leftmost position is larger than a's width...
		if( abs(bLeft - aLeft) < abs(aRight - aLeft) )
		{
			// They are obviosuly touching
			return true;
		}
	}
	// If box B is on the left...
	else if(bLeft < aLeft)
	{
		// if the difference between a and b's leftmost position is larger than b's width...
		if( abs(aLeft - bLeft) < abs(bRight - bLeft) )
		{
			// They are obviosuly touching
			return true;
		}
	}
	// If they are on the same location...
	else
	{
		// They are obviosuly touching
		return true;
	}
	
	// Didn't fulfill requirements?  Then they are not touching.
	return false;
}
//request that fire's hitbox be deflected and team altered
void requestFireDeflect(HitBox box, char * team)
{
	fire.requestFireDeflect(box, team);
}