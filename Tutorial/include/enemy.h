/*Programmer: Sean Higgins 
*/
#ifndef ENEMY_H
#define ENEMY_H

#include "include\structs.h"

struct singleEnemy
{
	Point pos;
	Point rotate;
	Point direction;
	float directionTime;
	int HP;
	int maxHP;
	float speed;
	float accel;
	int attack;
	float fireTime;//time usage of fire
	float maxFireTime;//amount of time required to charge until next shot
	HitBox hitbox;
	float damageTime;
};

class Enemy
{
	private:
		//functions

	public:
		//definitions
		int maxEnemy;
		singleEnemy en[20];

		//functions
		Enemy();
		void damageCheck(int i);
		void updateHitBox(int i);
		void EnemyRequest();
		void EnemyRequest(Point pos, int hp, float fireTime, float accel, int attack, float speed);
		void handleEnemy();
		void enemyBoundary(int i);
		int emptyEnemy();
		void clearEnemy(int i);
		int currentAmount();
};

#endif