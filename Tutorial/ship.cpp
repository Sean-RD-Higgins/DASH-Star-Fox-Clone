//package includes
#include <math.h>
#include <stdlib.h>

#include "include\ship.h"

// Constructor
Arwing::Arwing()
{
	init();
}

// Initialize everything
void Arwing::init()
{
	pos = newPoint(0.0f, 0.0f, 0.0f);
	HP = 100.0f;
	maxHP = 100.0f;
	maxAccel = 0.007f;
	maxBoostAccel = 0.006f;
	fireTime = 0.0f;
	maxFireTime = 50.0f;
	rotate = newPoint(0,0,0);
	accel = 0.0f;
	railAccel = 0.005f;
	rollTime = 0.0f;
	throttle = 0;
	bearing = 0;
	boundBox.Left = -2.2f;
	boundBox.Right = 2.2f;
	boundBox.Top = +1.8f;
	boundBox.Bottom = -1.6f;
	boundBox.Back = -10.0f;
	boundBox.Front = -5.0f;
	updateHitBoxes();
	damageTime = 0.0f;
	player = 1;
}
// keycheck
bool Arwing::keychk(unsigned char keyc, int i)
{
	return (keyc == key[player - 1][i]);
}

// reacts to new keypress
void Arwing::pressReaction(unsigned char keypress, int button)
{
	// We don't care if we're dead
	if(HP < 0) return;

	// if not rolling.. we can use bearing or rolls
	if(rollTime <= 0.0f)
	{
		if(keychk(keypress, bearLeftKey)) bearing = -1;
		else if(keychk(keypress, bearRightKey)) bearing = 1;
	}

	// check for firing
	if(fireTime <= 0.0f)
	{
		//LEFT=0 MIDDLE=1 RIGHT=2
		if(button == 0)
		{
			fireTime = maxFireTime;
			fireRequest(pos, crosshair[player - 1], team , 5, 0.05f, 0.1f);
		}
	}

	//chedck for throttle
	if(keychk(keypress, boostKey)) throttle = 1;
	else if(keychk(keypress, brakeKey)) throttle = -1;
}
//reacts to new keyrelease
void Arwing::releaseReaction(unsigned char keyrel, int button)
{
	// We're dead, why do we care?
	if(HP < 0) return;

	//if not rolling can bearing break or roll again
	if(rollTime < 1.0f)
	{
		if(keychk(keyrel, bearLeftKey)) 				
		{
			if(bearing != 1) bearing = 0;
		}
		else if(keychk(keyrel, bearRightKey))
		{
			if(bearing != -1) bearing = 0;
		}

		if(keychk(keyrel, rollLeftKey)) {bearing = -1;rollTime = 300.0f;}
		else if(keychk(keyrel, rollRightKey)){bearing = 1;rollTime = 300.0f;}
	}
	
	//other key release checks
	if(keychk(keyrel, boostKey))
	{
		//ensure to not break brake if boost is released
		if(throttle != -1) throttle = 0;
	}
	else if(keychk(keyrel, brakeKey))
	{
		//ensure to not break boost if brake is released
		if(throttle != 1) throttle = 0;
	}
}

//add or subtract acceleration to the ship
void Arwing::accelAdd(float inc)
{
	// Let's increase the accelleration rate
	accel += inc;
	// Cap the accel while throttling or not bearing, different speeds after all.
	if((throttle == 1 || bearing != 0) && accel > maxBoostAccel) accel = maxBoostAccel;
	// Cap the accel while not throttling
	else if(throttle == 0 && accel > maxAccel) accel = maxAccel;
	// Under Cap the accel 
	else if(accel < 0.003f) accel = 0.003f;
}
//update hitbox location
void Arwing::updateHitBoxes()
{
	// Wings
	// The hitbox for the wings are different if bearing is changed
	if(bearing == 0)
	{
		hitbox[0].box.FrontTopLeft =    	newPoint(-1.1f,+0.0f,+0.5f);
		hitbox[0].box.BackBottomRight = 	newPoint(+1.1f,-0.2f,+0.3f);
	}
	else
	{
		hitbox[0].box.FrontTopLeft =    	newPoint(+0.1f,-1.1f,+0.5f);
		hitbox[0].box.BackBottomRight = 	newPoint(-0.1f,+1.1f,+0.3f);
	}
	hitbox[0].box.FrontTopLeft.x += pos.x;
	hitbox[0].box.FrontTopLeft.y += pos.y;
	hitbox[0].box.FrontTopLeft.z += pos.z;
	hitbox[0].box.BackBottomRight.x += pos.x;
	hitbox[0].box.BackBottomRight.y += pos.y;
	hitbox[0].box.BackBottomRight.z += pos.z;

	// Body
	hitbox[1].box.FrontTopLeft =    	 newPoint(-0.2f,+0.1f,+0.6f);
	hitbox[1].box.BackBottomRight = 	 newPoint(+0.2f,-0.1f,-0.7f);
	hitbox[1].box.FrontTopLeft.x += pos.x;
	hitbox[1].box.FrontTopLeft.y += pos.y;
	hitbox[1].box.FrontTopLeft.z += pos.z;
	hitbox[1].box.BackBottomRight.x += pos.x;
	hitbox[1].box.BackBottomRight.y += pos.y;
	hitbox[1].box.BackBottomRight.z += pos.z;
}
//Damage effect
void Arwing::checkDamage()
{
	// If we are not under any damage...
	if(damageTime < 1.0f)
	{
		// Check for a collision from our hitbox
		HitBox damage = detectCollision(hitbox[0]);
		// If the collision returns an enemy team name found...
		if(damage.team != "False")
		{
			// ... and if the ship's roll time is already over
			if(rollTime < 3.0f)
			{
				// ... took a hit of damage, let's set our invincible counter
				HP--;
				damageTime = 500.0f;
			}
			// ... they see me rooooollin'... reflectin'... and its ooooon my team
			else requestFireDeflect(damage, team);
		}
		else
		{
			// if our hitbox was a failure, check the other hitbox... should really be in a while loop for hitbox array length.  TODO
			damage = detectCollision(hitbox[1]);
			// If the collision returns an enemy team name found...
			if(damage.team != "False")
			{
				// ... and if the ship's roll time is already over
				if(rollTime < 3.0f)
				{
					// ... took a hit of damage, let's set our invincible counter
					HP--;
					damageTime = 500.0f;
				}
				// ... they see me rooooollin'... reflectin'... and its ooooon my team
				else requestFireDeflect(damage, team);
			}
		}
	}
	else
	{
		// ... invincible time is running out
		damageTime -= (float) deltaGameTime;

		//spaztic ship time!
		rotate.x += (-0.4f + randFloat() * 0.8f) * (float)deltaGameTime;
		rotate.y += (-0.4f + randFloat() * 0.8f) * (float)deltaGameTime;
		rotate.z += (-0.4f + randFloat() * 0.8f) * (float)deltaGameTime;
	}
}

// Ensure Arwing's position is within all boundaries
void Arwing::boundaryCheck()
{
	// Cap the ship's location at it's boundaries
	pos.x = (pos.x > boundBox.Right)?boundBox.Right:pos.x;
	pos.x = (pos.x < boundBox.Left)?boundBox.Left:pos.x;
	pos.y = (pos.y > boundBox.Top)?boundBox.Top:pos.y;
	pos.y = (pos.y < boundBox.Bottom)?boundBox.Bottom:pos.y;
	pos.z = (pos.z > boundBox.Front)?boundBox.Front:pos.z;
	pos.z = (pos.z < boundBox.Back)?boundBox.Back:pos.z;
}

// Ensure both rotates are range of num
void Arwing::capRotates()
{
	// You can only rotate SO WELL in a certain direction
	if(abs(rotate.y) > 30.0f) rotate.y = sign(rotate.y) * 30.0f;
	if(abs(rotate.x) > 70.0f) rotate.x = sign(rotate.x) * 70.0f;
	if(abs(rotate.z) > 360.0f) rotate.z -= 360 * sign(rotate.z);
}

//effects of throttle
void Arwing::throttleEffect()
{
	// Throttling
	if(throttle == 1)
	{
		//move along rail faster, movements are faster, move away from screen
		accelAdd(0.001f);
		railAccel = 0.02f;
		pos.z -= (float) (0.01f * deltaGameTime);
	}
	// Back throttling
	else if(throttle == -1)
	{
		//move along rail slower, movements are slower, move towards screen
		accelAdd(-0.001f);
		railAccel = 0.005f;
		pos.z += (float) (0.01f * deltaGameTime);
	}
	// Normal speeds
	else railAccel = 0.01f;
	crosshair[player - 1].z = pos.z - 7;
	cursor[player - 1].z = pos.z - 15;
}

// Allows the effects of bearing on accelleration and direction
void Arwing::bearingEffect()
{
	//accelerate towards crosshair based on bearing
	if(bearing == -1)
	{
		//ship rotate left if bearing left without roll
		if(rollTime < 1.0f)
		{
			if(rotate.z < 80) rotate.z += 1.0f * (float)deltaGameTime;
			else if(rotate.z < 90) rotate.z += 0.1f * (float)deltaGameTime;
			else rotate.z = 90.0f;
		}
		//accel left, deccel right
		if(crosshair[player - 1].x < pos.x) accelAdd(0.005f);
		else
		{
			accelAdd(-1.0f);
			accelAdd(0.002f);
		}
	}
	else if(bearing == 1)
	{
		//ship rotate right if bearing not rolling
		if(rollTime < 1.0f)
		{
			if(rotate.z > -80) rotate.z -= 1.0f * (float)deltaGameTime;
			else if(rotate.z > -90) rotate.z -= 0.1f * (float)deltaGameTime;
			else rotate.z = -90;
		}

		//accel right, deccel left
		if(crosshair[player - 1].x > pos.x) accelAdd(0.005f);
		else
		{
			accelAdd(-1.0f);
			accelAdd(0.002f);
		}
	}
	else accelAdd(0.00007f);
}

// Allows the effects of time to affect firing
void Arwing::fireEffect()
{
	if(fireTime > 0.0f) fireTime -= (float)deltaGameTime;
}

// Handle idle
void Arwing::handleIdle()
{
	followCrosshair();
	faceCrosshair();
	updateHitBoxes();
	checkDamage();
}

// Forces the ship to follow crosshairs
void Arwing::followCrosshair()
{
	//perform distance direction calls
	float distance = pointDistance( pos.x, pos.y, crosshair[player - 1].x, crosshair[player - 1].y );
	float direction = pointDirection( pos.x, pos.y, crosshair[player - 1].x, crosshair[player - 1].y );

	// allow the effects of bearing to change acceleration and direction
	bearingEffect();

	// throttle affects speed
	throttleEffect();
	
	// handle all firing effects
	fireEffect();
	
	// go towards the crosshair
	if(distance > 0.3f * sizeIndex())
	{
		pos.x -= (float) (sin(direction) * accel * sizeIndex() * deltaGameTime);
		pos.y -= (float) (cos(direction) * accel * sizeIndex() * deltaGameTime);
	}
	// if close enough: snap!
	else if(distance > 0.001f)
	{
		accelAdd(-1);
		pos.x -= sin(direction) * accel * sizeIndex();
		pos.y -= cos(direction) * accel * sizeIndex();
	}
	
	//cap all direction
	boundaryCheck();
}

// face the crosshair[player - 1] by rotating X and Y
void Arwing::faceCrosshair()
{
	if(damageTime <= 1.0f)
	{
		//Thread Spindle Rotate towards the crosshair[player - 1], 
		rotate.y = sin(pointDirection(pos.x,pos.z,crosshair[player - 1].x,crosshair[player - 1].z)) * 90.0f;
		if(bearing == 0)
		{
			//Rolling Pin Rotate towards the crosshair[player - 1], 
			rotate.x = - cos(pointDirection(pos.z,pos.y,crosshair[player - 1].z,crosshair[player - 1].y)) * 90.0f;
		
			//Box Fan rotation for the Strafe effect
			rotate.z -= 0.1f * sign(rotate.z) * (float)deltaGameTime;
		
			//Box Fan rotation for the Strafe effect faster if rotate is too far
			if(abs(30 * sign(rotate.y) - rotate.z) > 30)
			{
				rotate.z -= 0.1f * sign(rotate.z) * (float)deltaGameTime;
			}
		
			//if trying to turn too wide, Box Fan rotate instead for nicer look
			if(abs(rotate.y) > 15.0f)
			{
				//go towards the absolute Box Fan Rotate
				rotate.z += 0.1f * sign(rotate.y) * (float)deltaGameTime;
				//make the Box Fan Rotate be absolute
				if(abs(rotate.z) > 30 && abs(rotate.z) < 35 && sign(rotate.z) == sign(rotate.y))
				{
					rotate.z = 30.0f * sign(rotate.y);
				}
				//go towards the absolute Box Fan Rotate
				else if(abs(rotate.z) > 35.0f)
				{
					rotate.z -= 0.5f * sign(rotate.y) * (float)deltaGameTime;
				}
			}
		}
		else
		{
			//Rolling Pin Rotate towards the crosshair[player - 1]
			rotate.x = - cos(pointDirection(pos.z,pos.y,crosshair[player - 1].z,crosshair[player - 1].y)) * 70.0f;
		
			//Box Fan Rotate in roll direction
			if(rollTime > 1.0f)
			{
				rotate.z -= 4.0f * bearing * (float)deltaGameTime;
				rollTime -= (float)deltaGameTime;
			}

			//bearing break if rolltime is done
			if(rollTime > 1.0f && rollTime < 10.0f)
			{
				bearing = 0;
				rotate.z = 0;
				rollTime = 0.0f;
			}
		}
		capRotates();
	}
}