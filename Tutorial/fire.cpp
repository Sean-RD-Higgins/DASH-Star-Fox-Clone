//package includes
#include <math.h>
#include <stdlib.h>

//prototype
#include "include\structs.h"
#include "include\functions.h"
#include "include\fire.h"

//intialize all ship data
Fire::Fire()
{
	//ensure everyone knows how many fires at once
	maxFire = 30;

	//clear all shots
	for(int i = 0;i < maxFire;i++)
	{
		clearShot(i);
	}
}
//request for a fire to be shot
void Fire::fireRequest(Point pos, Point direction, char * team, int attack, float speed, float AoE)
{
	int i = emptyFire();
	shot[i].pos = pos;
	shot[i].direction = direction;
	shot[i].team = team;
	shot[i].attack = attack;
	shot[i].time = 0;
	shot[i].speed = speed;
	shot[i].AoE = AoE;
	shot[i].hitbox.team = shot[i].team;
}
//handle normal firing procedure
void Fire::handleFire()
{
	for(int i = 0;i < maxFire;i++)
	{
		//empty some shots
		if(shot[i].pos.z < -50 || shot[i].pos.z > 10 || shot[i].time > 2000)
		{
			clearShot(i);
		}
		else if(shot[i].team != "Empty")
		{
			shot[i].time += (float) deltaGameTime;
			shot[i].pos.x += (float) (shot[i].speed * shot[i].direction.x * deltaGameTime);
			shot[i].pos.y += (float) (shot[i].speed * shot[i].direction.y * deltaGameTime);
			shot[i].pos.z -= (float) (shot[i].speed * shot[i].direction.z * deltaGameTime);
			updateHitBox(i);
		}
	}
}
//update hitbox
void Fire::updateHitBox(int i)
{
	shot[i].hitbox.box.FrontTopLeft =    newPoint(-0.2f,+0.2f,+0.8f);
	shot[i].hitbox.box.BackBottomRight = newPoint(+0.2f,-0.2f,-0.2f);
	shot[i].hitbox.box.FrontTopLeft.x += shot[i].pos.x;
	shot[i].hitbox.box.FrontTopLeft.y += shot[i].pos.y;
	shot[i].hitbox.box.FrontTopLeft.z += shot[i].pos.z;
	shot[i].hitbox.box.BackBottomRight.x += shot[i].pos.x;
	shot[i].hitbox.box.BackBottomRight.y += shot[i].pos.y;
	shot[i].hitbox.box.BackBottomRight.z += shot[i].pos.z;
}
//finds and returns the next empty index in the array for shots fired
int Fire::emptyFire()
{
	//looks for an empty space inthe array first while calculating which has been on the field the longest
	int longest = 0;
	for(int i = 0;i < maxFire;i++)
	{
		if(shot[i].team == "Empty")
		{
			return i;
		}
		if(shot[i].time > shot[longest].time)
		{
			longest = i;
		}
	}

	//apparently full; let's take out the one that's been on the field the longest
	return longest;
}
//makes a shot completely empty
void Fire::clearShot(int i)
{
	shot[i].pos = newPoint(0.0f,0.0f,0.0f);
	shot[i].direction = newPoint(0,0,0);
	shot[i].team = "Empty";
	shot[i].attack = 0;
	shot[i].speed = 0.0f;
	shot[i].AoE = 0.0f;
}
//request fire be deflected
void Fire::requestFireDeflect(HitBox box, char * team)
{
	//inits
	bool found = false;
	int i;

	//find the same shot with the box
	for(i = 0;i < maxFire && found;i++)
	{
		if(box.box.FrontTopLeft.x == shot[i].hitbox.box.FrontTopLeft.x &&
		box.box.FrontTopLeft.y == shot[i].hitbox.box.FrontTopLeft.y &&
		box.box.FrontTopLeft.z == shot[i].hitbox.box.FrontTopLeft.z &&
		box.box.BackBottomRight.x == shot[i].hitbox.box.BackBottomRight.x &&
		box.box.BackBottomRight.y == shot[i].hitbox.box.BackBottomRight.y &&
		box.box.BackBottomRight.z == shot[i].hitbox.box.BackBottomRight.z &&
		box.team == shot[i].hitbox.team)	found = true;
	}

	//didnt find it
	if(found == false) return;

	//change the properties
	shot[i].team = team;
	shot[i].direction.x += 180;
	shot[i].direction.y += 180;
	shot[i].direction.z += 180;
}