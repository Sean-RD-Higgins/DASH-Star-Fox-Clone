/*Programmer: Sean Higgins 
*/
#ifndef SHIP_H
#define SHIP_H

//package includes
#include <math.h>
#include <stdlib.h>

#include "include\structs.h"
#include "include\functions.h"

class Arwing
{
	private:
		//functions
		void init();
		void capRotates();
		void boundaryCheck();
		void fireEffect();
		void bearingEffect();
		void throttleEffect();
		bool keychk(unsigned char a,int b);

	public:
		//definitions
		Point pos;
		Point rotate;
		float HP;
		float maxHP;
		float accel;
		float maxAccel;
		float maxBoostAccel;
		char player;
		float fireTime;//time usage of fire
		float maxFireTime;//amount of time required to charge until next shot
		float railAccel;
		char bearing;// -1 for left, 1 for right
		char throttle;// -1 for brake, 1 for boost
		float rollTime;//time left for a roll
		float damageTime;
		char * team;
		BoundBox boundBox;
		HitBox hitbox[2];

		//functions
		Arwing();//constructor
		void updateHitBoxes();
		void accelAdd(float inc);//add or subtract acceleration to the ship
		void followCrosshair();//forces the ship to follow crosshairs
		void faceCrosshair();//rotate X,Y to face the crosshairs.
		void pressReaction(unsigned char key, int button);
		void releaseReaction(unsigned char key, int button);
		void handleIdle();
		void checkDamage();
};

#endif