/*OPENGL TUTORIAL PROJECT
 *Programmer: Sean Higgins
 *imageloader Programmer: videotutorialsrock.com
 *glpng Programmer: Ben Wyatt
*/

//includes
#include <iostream>
#include <string>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "include\structs.h"
#include "include\imageloader.h"
#include "include\functions.h"
#include "include\ship.h"
#include "include\drawing.h"

//====prototypes====
GLuint loadTexture(Image* image);
GLuint imageToTexture(const char * filename);
void initRendering();
void handleCrosshair(int i);
void shipFollow();
void containCursor(float * cursorx,float * cursory);
void handleMouse(int x, int y);
void handleKeypress(unsigned char key, int x, int y);
void handleResize(int w, int h);
void handleIdle();

//namespace
using namespace std;

//globals
float mouseSensitivity = 200.0f;
float keySensitivity = 0.8f;
float joySensitivity = 5000.0f;
float moveSpeed = 1.1f;
Point varMove = {0,0,0};
float cursorSensitive = 1.0f;
double currentGameTime;
double prevGameTime;

//Handles all key presses
void handleJoystick(unsigned int buttonMask, int x, int y, int z)
{
	if(y < -300) cursor[1].y += -y / joySensitivity;
	else if(y > 300) cursor[1].y -= y / joySensitivity;
	if(x > 300) cursor[1].x += x / joySensitivity;
	else if(x < -300) cursor[1].x -= -x / joySensitivity;
	if(buttonMask > 0) ship2.pressReaction(NULL, 0);

	containCursor(&cursor[1].x,&cursor[1].y);
	containCursor(&crosshair[1].x,&crosshair[1].y);
}

//Handles all key presses
void handleKeypress(unsigned char key, int x, int y) 
{	
	//check for ESC key press
	if(key == 27) exit(EXIT_SUCCESS);

	KeyDown[key] = 1;
	ship.pressReaction(key, -1);
	ship2.pressReaction(key, -1);

	//temp p2 keys
	if(key == 't'){cursor[1].y += keySensitivity;}
	else if(key == 'g'){cursor[1].y -= keySensitivity;}
	if(key == 'h') cursor[1].x += keySensitivity;
	else if(key == 'f') cursor[1].x -= keySensitivity;
	if(key == 'm') ship2.pressReaction(NULL, 0);
	containCursor(&cursor[1].x,&cursor[1].y);
	containCursor(&crosshair[1].x,&crosshair[1].y);
}

//Handles all key releases
void handleKeyrelease(unsigned char key, int x, int y)
{
	KeyDown[key] = 0;
	ship.releaseReaction(key, -1);
	ship2.releaseReaction(key, -1);
	
	if(key == 'm') ship2.releaseReaction(NULL, 0);

}

//handles all cursor movement
void handleMouse(int x, int y)
{
	if(320 != x || 240 != y)
	{
		cursor[0].x += (x - 320) / mouseSensitivity;
		cursor[0].y -= (y - 240) / mouseSensitivity;
		glutWarpPointer(320, 240);
	}
	containCursor(&cursor[0].x,&cursor[0].y);
	containCursor(&crosshair[0].x,&crosshair[0].y);
}

//handles all mouse clicks
void handleClick(int button, int state, int x, int y) 
{
	switch(state)
	{
		case GLUT_DOWN:
			ship.pressReaction(NULL, button);
		break;
		case GLUT_UP:
			ship.releaseReaction(NULL, button);
		break;
	}
}

//forces crosshairs to follow cursor
void handleCrosshair(int i)
{
	float distance = pointDistance(crosshair[i].x,crosshair[i].y,cursor[i].x,cursor[i].y);
	float direction = pointDirection(crosshair[i].x,crosshair[i].y,cursor[i].x,cursor[i].y);
	if(distance > 0.3f * sizeIndex())
	{
		crosshair[i].x -= sin(direction) * 0.015f * sizeIndex();
		crosshair[i].y -= cos(direction) * 0.015f * sizeIndex();
	}
	else
	{
		crosshair[i].x -= sin(direction) * 0.003f * sizeIndex();
		crosshair[i].y -= cos(direction) * 0.003f * sizeIndex();
	}
}

//Puts any cursor within a Frame, eliminating out of bounds issues
void containCursor(float * cursorx,float * cursory)
{
	*cursorx = (*cursorx < crosshairBoundBox.Left)?crosshairBoundBox.Left:*cursorx;
	*cursorx = (*cursorx > crosshairBoundBox.Right)?crosshairBoundBox.Right:*cursorx;
	*cursory = (*cursory < crosshairBoundBox.Bottom)?crosshairBoundBox.Bottom:*cursory;
	*cursory = (*cursory > crosshairBoundBox.Top)?crosshairBoundBox.Top:*cursory;
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) 
{
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

//Opens the specifies file, Calls loadTexture, deletes the file in memory, returns the id of the texture
GLuint imageToTexture(const char * filename)
{

	Image* image = loadBMP(filename);
	GLuint textId;
	textId = loadTexture(image);
	delete image;
	return textId;
}

//preloads alll textures and camera related graphics
void initRendering() 
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	_tid_star = imageToTexture("star.bmp");
	_tid_crosshair = imageToTexture("crosshair.bmp");
	_tid_outerCrosshair = imageToTexture("outerCrosshair.bmp");
	_tid_white = imageToTexture("white.bmp");
	_tid_blue = imageToTexture("blue.bmp");
	_tid_red = imageToTexture("red.bmp");
	_tid_yellow = imageToTexture("yellow.bmp");
	_tid_black = imageToTexture("black.bmp");

	w = 800;
	h = 600;
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	screenWidth = (float)w;
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);

}

//reproject the viewing window if user resizes window
void handleResize(int w, int h) 
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	screenWidth = (float)w;
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

//handle functions when Idle
void handleIdle()
{
	// let's get the amount of time that has passed since the last frame
	currentGameTime = GetCurrentTime();
	deltaGameTime = currentGameTime - prevGameTime;
	prevGameTime = currentGameTime;

	// increase timer but we won't go overboard
	timer += (float)deltaGameTime;

	// Used for debugging.  ignore
	//debug[0] = deltaGameTime;
	//for(int i = 0;i <= 0;i++){if(timer == 1){cout << i << ":" << debug[i] << endl;}}

	// Let's add an enemy only at a guided random interval
	if((int)timer % 1000 == 1 && randFloat() < 0.5 && enemy.currentAmount() < 4)
	{
		timer = 0.0f;
		enemy.EnemyRequest();
	}

	// Let the attack manager do it's thing
	fire.handleFire();

	// Enemy cycle
	enemy.handleEnemy();

	// Stars... they fly
	handleStars();

	// Player 1 (0) and 2 (1) crosshairs
	handleCrosshair(0);
	handleCrosshair(1);

	// Player 1 and 2's ship cycle
	ship.handleIdle();
	ship2.handleIdle();

	// Draw time.
	drawScene();
}

//init keys
void initEverything()
{
	//keys
	key[0][bearLeftKey] = 'a';
	key[0][bearRightKey] = 'd';
	key[0][rollLeftKey] = 'q';
	key[0][rollRightKey] = 'e';
	key[0][boostKey] = 'w';
	key[0][brakeKey] = 's';

	key[1][bearLeftKey] = 'j';
	key[1][bearRightKey] = 'l';
	key[1][rollLeftKey] = 'u';
	key[1][rollRightKey] = 'o';
	key[1][boostKey] = 'i';
	key[1][brakeKey] = 'k';

	//crosshair cursor
	crosshair[0] = newPoint(-1,-2,0);
	crosshair[1] = newPoint(1,-2,0);
	cursor[0] = newPoint(-1,-2,0);
	cursor[1] = newPoint(1,-2,0);

	//drawing
	initStars();
	initDrawItems();

	//ship inits
	ship.pos = newPoint(-1, -2, -5);
	ship2.pos = newPoint(1, -2, -5);
	ship.player = 1;
	ship2.player = 2;
	ship.team = "Team1";
	ship2.team = "Team2";
	ship.hitbox[0].team = ship.team;
	ship.hitbox[1].team = ship.team;
	ship2.hitbox[0].team = ship2.team;
	ship2.hitbox[1].team = ship2.team;

	//game time
	deltaGameTime = 0;
	currentGameTime = GetCurrentTime();
	prevGameTime = currentGameTime;
}

//main module
int main(int argc, char** argv) 
{
	initEverything();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutGameModeString("800x600:16@60");
	glutEnterGameMode();

	initRendering();

	glutSetCursor(GLUT_CURSOR_NONE);
	glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
	
	// Callback functions
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutKeyboardUpFunc(handleKeyrelease);
	glutJoystickFunc(handleJoystick, 50);
	glutMotionFunc(handleMouse);
	glutPassiveMotionFunc(handleMouse);
	glutMouseFunc(handleClick);

	glutIdleFunc(handleIdle);
	
	glutMainLoop();
	return 0;
} 