//package includes
#include <math.h>
#include <stdlib.h>

// Prototype
#include "include\structs.h"
#include "include\functions.h"
#include "include\enemy.h"

// Intialize all enemy data
Enemy::Enemy()
{
	maxEnemy = 20;
	for(int i = 0;i < maxEnemy;i++) clearEnemy(i);
}

// Request a random enemy be created with all random stats
void Enemy::EnemyRequest()
{
	int i = emptyEnemy();
	en[i].pos = newPoint(-2 + randFloat() * 4, -2 + randFloat() * 4, - randFloat() * 15 - 6);
	en[i].rotate = newPoint(0,0,0);
	en[i].direction = newPoint(0,0,0);
	en[i].directionTime = 0.0f;
	en[i].HP = (int)(2 + randFloat() * 3);
	en[i].maxHP = en[i].HP;
	en[i].fireTime = 0;
	en[i].maxFireTime = 200.0f + randFloat() * 200.0f;
	en[i].speed = randFloat() * 0.05f;
	en[i].attack = 3;
	en[i].accel = randFloat() * 0.05f;
	en[i].hitbox.team = "Enemy";
	en[i].damageTime = 0;
	updateHitBox(i);
}

// Request an enemy be created
void Enemy::EnemyRequest(Point pos, int hp, float fireTime, float accel, int attack, float speed)
{
	int i = emptyEnemy();
	en[i].pos = pos;
	en[i].rotate = newPoint(0,0,0);
	en[i].direction = newPoint(0,0,0);
	en[i].directionTime = 0.0f;
	en[i].HP = hp;
	en[i].maxHP = hp;
	en[i].fireTime = 0;
	en[i].maxFireTime = fireTime;
	en[i].speed = speed;
	en[i].attack = attack;
	en[i].accel = accel;
	en[i].hitbox.team = "Enemy";
	en[i].damageTime = 0;
	updateHitBox(i);
}

//update hitbox location
void Enemy::updateHitBox(int i)
{
	en[i].hitbox.box.FrontTopLeft =    newPoint(-0.7f,+0.2f,+0.7f);
	en[i].hitbox.box.BackBottomRight = newPoint(+0.7f,-0.4f,-0.7f);
	en[i].hitbox.box.FrontTopLeft.x += en[i].pos.x;
	en[i].hitbox.box.FrontTopLeft.y += en[i].pos.y;
	en[i].hitbox.box.FrontTopLeft.z += en[i].pos.z;
	en[i].hitbox.box.BackBottomRight.x += en[i].pos.x;
	en[i].hitbox.box.BackBottomRight.y += en[i].pos.y;
	en[i].hitbox.box.BackBottomRight.z += en[i].pos.z;
}

// Ensure the enemy is within bounds
void Enemy::enemyBoundary(int i)
{
	// Cap the ship's location at it's boundaries
	en[i].pos.x = (en[i].pos.x > crosshairBoundBox.Right)?crosshairBoundBox.Right:en[i].pos.x;
	en[i].pos.x = (en[i].pos.x < crosshairBoundBox.Left)?crosshairBoundBox.Left:en[i].pos.x;
	en[i].pos.y = (en[i].pos.y > crosshairBoundBox.Top)?crosshairBoundBox.Top:en[i].pos.y;
	en[i].pos.y = (en[i].pos.y < crosshairBoundBox.Bottom)?crosshairBoundBox.Bottom:en[i].pos.y;
	en[i].pos.z = (en[i].pos.z > crosshairBoundBox.Front)?crosshairBoundBox.Front:en[i].pos.z;
	en[i].pos.z = (en[i].pos.z < crosshairBoundBox.Back)?crosshairBoundBox.Back:en[i].pos.z;
}

// Regular maintenance
void Enemy::handleEnemy()
{
	for(int i = 0;i < maxEnemy;i++)
	{
		if(en[i].HP > 0)
		{
			//fly around
			en[i].pos.x += (float)(en[i].direction.x * en[i].speed * deltaGameTime);
			en[i].pos.y += (float)(en[i].direction.y * en[i].speed * deltaGameTime);
			en[i].pos.z += (float)(en[i].direction.z * en[i].speed * deltaGameTime);

			en[i].directionTime -= (float)deltaGameTime;

			// Choose a new direction when one has finished going in a direction for a certain time
			if(en[i].directionTime <= 0)
			{
				en[i].directionTime = 10 + (float)(randFloat() * 50.0f);
				en[i].direction.x = -1.0f + randFloat() * 2.0f;
				en[i].direction.y = -1.0f + randFloat() * 2.0f;
				en[i].direction.z = -0.05f + randFloat() * 0.1f;
			}
			enemyBoundary(i);

			//stare at ship
			if(en[i].damageTime < 2)
			{
				en[i].rotate.x = cos(pointDirection(en[i].pos.z,en[i].pos.y,ship.pos.z,ship.pos.y)) * 90;
				en[i].rotate.y = - sin(pointDirection(en[i].pos.x,en[i].pos.z,ship.pos.x,ship.pos.z)) * 90;
			}

			//check for damage
			updateHitBox(i);
			damageCheck(i);

			//fire at arwing
			if(en[i].fireTime < 1 && randFloat() < 0.1f && en[i].damageTime < 1)
			{
				Point aim = ship.pos;
				if(randFloat() > 0.5) aim = ship2.pos;
				aim.x += - 2 + randFloat() * 4;
				aim.y += - 2 + randFloat() * 4;
				aim.z += - 2 + randFloat() * 4;
				fireRequest(en[i].pos, aim, "Enemy" , en[i].attack, 0.02f, 0.2f);
				en[i].fireTime = en[i].maxFireTime;
			}
			else en[i].fireTime -= (float)deltaGameTime;
		}
	}
}

// Ensure all ships are not damaged
void Enemy::damageCheck(int i)
{
	if(en[i].damageTime < 1.0f)
	{
		HitBox damage = detectCollision(en[i].hitbox);
		if(damage.team != "False")
		{
			en[i].HP--;
			en[i].damageTime = 100;
		}
	}
	else
	{
		en[i].damageTime -= (float)deltaGameTime;
		//spaztic ship
		en[i].rotate.x += -3.0f + randFloat() * 6.0f;
		en[i].rotate.y += -3.0f + randFloat() * 6.0f;
		en[i].rotate.z += -3.0f + randFloat() * 6.0f;
	}
}

// Finds and returns the next empty index in the array for Enemies
int Enemy::emptyEnemy()
{
	for(int i = 0;i < maxEnemy;i++)
	{
		if(en[i].HP < 1) return i;
	}
	return 1 + (int)((maxEnemy - 1.0f) * randFloat());
}

// Makes a enemy completely empty
void Enemy::clearEnemy(int i)
{
	en[i].pos = newPoint(0.0f,0.0f,0.0f);
	en[i].rotate = newPoint(0.0f,0.0f,0.0f);
	en[i].HP = -1;
	en[i].maxHP = -1;
	en[i].accel = -1.0f;
	en[i].fireTime = -1.0f;//time usage of fire
	en[i].maxFireTime = -1.0f;//amount of time required to charge until next shot
	en[i].damageTime = 0.0f;//amount of time required to charge until next shot
}

// Return the amount of enemies there are right now
int Enemy::currentAmount()
{
	int count = 0;
	for(int i = 0; i < maxEnemy;i++)
	{
		if(en[i].HP > 1) count++;
	}
	return count;
}