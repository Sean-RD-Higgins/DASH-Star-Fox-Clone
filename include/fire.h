/*Programmer: Sean Higgins 
*/
#ifndef FIRE_H
#define FIRE_H

#include "include\structs.h"

struct Shot
{
	Point pos;
	Point direction;
	char * team;
	float time;
	int attack;
	float speed;
	float AoE;
	HitBox hitbox;
};

class Fire
{
	private:
		//functions

	public:
		//definitions
		int maxFire;
		Shot shot[30];

		//functions
		Fire();
		void fireRequest(Point from, Point direction, char * team, int attack, float speed, float AoE);
		void handleFire();
		void updateHitBox(int i);
		int emptyFire();
		void clearShot(int i);
		void requestFireDeflect(HitBox box, char * team);
};

#endif