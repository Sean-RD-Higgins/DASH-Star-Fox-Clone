#include "include\drawing.h"
#include "include\functions.h"

GLuint _tid_star; //The id of the texture
GLuint _tid_crosshair; //The texture id of the crosshair texture
GLuint _tid_outerCrosshair; //The texture id of the outer crosshair texture
GLuint _tid_white; //The texture id of the ship
GLuint _tid_blue; //the texture ID of a blue texture
GLuint _tid_black; //the texture ID of a blue texture
GLuint _tid_red; //the texture ID of a red texture
GLuint _tid_yellow; //the texture ID of a yellow texture
float timer = 0.0f;
float x,y,z,l,w,h;

//Draws a simple Texture Pyramid
void drawSimplePyramid(float l, GLuint texId)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);

	glNormal3f(0.0, 0.0f, 1.0f);
	
	//all from top view
	//left side of pyramid
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( -l, -l,-l);			// Top Right Of The Quad (Top)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-l, -l, l);			// Top Left Of The Quad (Top)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, l, 0);			// Bottom Left Of The Quad (Top)
	//right side
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(l, -l,-l);			// Top Right Of The Quad (Top)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(l, -l, l);			// Top Left Of The Quad (Top)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, l, 0);			// Bottom Left Of The Quad (Top)
	//up side
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( -l, -l,-l);			// Top Right Of The Quad (Top)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(l, -l,-l);			// Top Right Of The Quad (Top)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, l, 0);			// Bottom Left Of The Quad (Top)
	//down side
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-l, -l, l);			// Top Left Of The Quad (Top)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(l, -l, l);			// Top Left Of The Quad (Top)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(0, l, 0);			// Bottom Left Of The Quad (Top)
	glEnd();
	drawExtQuad(-l,-l,-l,l * 2,0,l * 2,texId);
}

//Draws a Textured Pyramid with specs
void drawCornersPyramid(PyramidCorners p, GLuint texId)
{
	//draw an open pyramid
	drawCornersOpenPyramid(p, texId);

	//very bottom
	glBegin(GL_QUADS);
	
		glNormal3f(p.front.x, p.front.y, p.front.z + 1);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(p.back.x,p.back.y,p.back.z);

		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(p.right.x, p.right.y, p.right.z);

		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(p.front.x,p.front.y,p.front.z);

		glTexCoord2f(0.0f, 1.0f);
		glVertex3f(p.left.x, p.left.y, p.left.z);

	glEnd();

}

//draws a textured open pyramid
void drawCornersOpenPyramid(PyramidCorners p, GLuint texId)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLES);

		glNormal3f(p.front.x, p.front.y, p.front.z + 1);

		//all from p.top. view
		//clockwise starting from "up" or "back"
		//p.back. corner
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(p.back.x,p.back.y,p.back.z);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(p.right.x, p.right.y, p.right.z);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(p.top.x, p.top.y, p.top.z);
		//p.right. corner
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(p.right.x, p.right.y, p.right.z);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(p.front.x,p.front.y,p.front.z);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(p.top.x, p.top.y, p.top.z);
		//p.front. corner
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(p.front.x,p.front.y,p.front.z);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(p.left.x, p.left.y, p.left.z);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(p.top.x, p.top.y, p.top.z);
		//down side
		glTexCoord2f(0.0f, 0.0f);
		glVertex3f(p.left.x, p.left.y, p.left.z);
		glTexCoord2f(1.0f, 0.0f);
		glVertex3f(p.back.x,p.back.y,p.back.z);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3f(p.top.x, p.top.y, p.top.z);
	glEnd();
}

//Draws a simple Texture Box
void drawSimpleCube(float l, GLuint texId)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);

	glNormal3f(0.0, 0.0f, 1.0f);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( l, l,-l);			// Top Right Of The Quad (Top)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-l, l,-l);			// Top Left Of The Quad (Top)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-l, l, l);			// Bottom Left Of The Quad (Top)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f( l, l, l);			// Bottom Right Of The Quad (Top)
	
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( l,-l, l);			// Top Right Of The Quad (Bottom)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-l,-l, l);			// Top Left Of The Quad (Bottom)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-l,-l,-l);			// Bottom Left Of The Quad (Bottom)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f( l,-l,-l);			// Bottom Right Of The Quad (Bottom)

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( l, l, l);			// Top Right Of The Quad (Front)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-l, l, l);			// Top Left Of The Quad (Front)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-l,-l, l);			// Bottom Left Of The Quad (Front)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f( l,-l, l);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( l,-l,-l);			// Bottom Left Of The Quad (Back)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-l,-l,-l);			// Bottom Right Of The Quad (Back)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-l, l,-l);			// Top Right Of The Quad (Back)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f( l, l,-l);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-l, l, l);			// Top Right Of The Quad (Left)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-l, l,-l);			// Top Left Of The Quad (Left)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-l,-l,-l);			// Bottom Left Of The Quad (Left)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-l,-l, l);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( l, l,-l);			// Top Right Of The Quad (Right)
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f( l, l, l);			// Top Left Of The Quad (Right)
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f( l,-l, l);			// Bottom Left Of The Quad (Right)
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f( l,-l,-l);

	glEnd();
}

// Draws a simple Texture Box
void drawExtCube(BoxCorners b, GLuint texId)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);

	glNormal3f(0.0, 0.0f, 1.0f);

	//TOP<
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(b.BackLeft.x, b.BackLeft.y, b.BackLeft.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(b.BackTop.x, b.BackTop.y, b.BackTop.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(b.FrontTop.x, b.FrontTop.y, b.FrontTop.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(b.FrontLeft.x, b.FrontLeft.y, b.FrontLeft.z);

	//RIGHT<
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(b.BackRight.x, b.BackRight.y, b.BackRight.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(b.BackTop.x, b.BackTop.y, b.BackTop.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(b.FrontTop.x, b.FrontTop.y, b.FrontTop.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(b.FrontRight.x, b.FrontRight.y, b.FrontRight.z);

	//FRONT<
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(b.FrontTop.x, b.FrontTop.y, b.FrontTop.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(b.FrontRight.x, b.FrontRight.y, b.FrontRight.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(b.FrontBottom.x, b.FrontBottom.y, b.FrontBottom.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(b.FrontLeft.x, b.FrontLeft.y, b.FrontLeft.z);

	//BOTTOM
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(b.BackRight.x, b.BackRight.y, b.BackRight.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(b.FrontRight.x, b.FrontRight.y, b.FrontRight.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(b.FrontBottom.x, b.FrontBottom.y, b.FrontBottom.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(b.BackBottom.x, b.BackBottom.y, b.BackBottom.z);

	//BACK<
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(b.BackTop.x, b.BackTop.y, b.BackTop.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(b.BackRight.x, b.BackRight.y, b.BackRight.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(b.BackBottom.x, b.BackBottom.y, b.BackBottom.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(b.BackLeft.x, b.BackLeft.y, b.BackLeft.z);

	//LEFT<
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(b.BackLeft.x, b.BackLeft.y, b.BackLeft.z);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(b.FrontLeft.x, b.FrontLeft.y, b.FrontLeft.z);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(b.FrontBottom.x, b.FrontBottom.y, b.FrontBottom.z);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(b.BackBottom.x, b.BackBottom.y, b.BackBottom.z);

	glEnd();
}

//Draws a Quad with specified parameters
void drawExtQuad(float x,float y,float z,float l,float w, float h, GLuint texId)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texId);
	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	
	glNormal3f(0.0, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x, y, z);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(x + l, y, z);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(x + l, y + w, z + h);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x, y + w, z + h);

	glEnd();
}

//inits the camera and the light
void initCamLight()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	//center camera
	glTranslatef(0.0f, 1.0f, -6.0f);
	
	//Lights
	GLfloat ambientLight[] = {0.2f, 0.2f, 0.2f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat directedLight[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat directedLightPos[] = {-10.0f, 15.0f, 20.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);	
}

//invert all the pyramid corners X values by *-1
PyramidCorners invertPyramidCornersX(PyramidCorners pc)
{
	pc.back.x *= -1;
	pc.right.x *= -1;
	pc.front.x *= -1;
	pc.left.x *= -1;
	pc.top.x *= -1;
	return pc;
}

//Arwing, you draw it
void drawArwing()
{
	if(ship.HP < 0)
	{
		return;
	}

	glLoadIdentity();
	glTranslatef(ship.pos.x + 0.1f,ship.pos.y - 0.1f,ship.pos.z);
	glRotatef(ship.rotate.x,1.0f,0.0f,0.0f);
	glRotatef(ship.rotate.y,0.0f,1.0f,0.0f);
	glRotatef(ship.rotate.z,0.0f,0.0f,1.0f);

	GLuint shipTex = _tid_white;
	if(ship.damageTime > 1 && (int)timer % 30 != 1)
	{
		shipTex = _tid_red;
	}

		/*Arwing*/
	drawCornersPyramid(leftWing, shipTex);
	drawCornersPyramid(rightWing, shipTex);
	drawCornersOpenPyramid(leftTopJet, _tid_blue);
	drawCornersOpenPyramid(rightTopJet, _tid_blue);
	drawCornersOpenPyramid(leftBottomJet, _tid_blue);
	drawCornersOpenPyramid(rightBottomJet, _tid_blue);
	drawCornersOpenPyramid(topBody, shipTex);
	drawCornersPyramid(bottomBody, shipTex);
		/*Left Jet Flame*/
	drawExtQuad(-0.25f - ship.throttle * 0.025f,-0.12f - ship.throttle * 0.025f,0.6f + ship.throttle * 0.05f, 0.1f + ship.throttle * 0.05f,0.15f + ship.throttle * 0.05f,0, (_tid_red * (((int)timer % 80) < 40)) + (_tid_yellow * (((int)timer % 80) >= 40)));
		/*Right Jet Flame*/
	drawExtQuad(0.15f - ship.throttle * 0.025f,-0.12f - ship.throttle * 0.025f,0.6f + ship.throttle * 0.05f, 0.1f + ship.throttle * 0.05f,0.15f + ship.throttle * 0.05f,0, (_tid_red * (((int)timer % 80) < 40)) + (_tid_yellow * (((int)timer % 80) >= 40)));
}

//Arwing2, you draw it
void drawArwing2()
{
	if(ship2.HP < 0) return;

	glLoadIdentity();
	glTranslatef(ship2.pos.x + 0.1f,ship2.pos.y - 0.1f,ship2.pos.z);
	glRotatef(ship2.rotate.x,1.0f,0.0f,0.0f);
	glRotatef(ship2.rotate.y,0.0f,1.0f,0.0f);
	glRotatef(ship2.rotate.z,0.0f,0.0f,1.0f);

	GLuint ship2Tex = _tid_white;
	if(ship2.damageTime > 1 && (int)timer % 30 != 1) ship2Tex = _tid_red;

		/*Arwing*/
	drawCornersPyramid(leftWing, ship2Tex);
	drawCornersPyramid(rightWing, ship2Tex);
	drawCornersOpenPyramid(leftTopJet, _tid_blue);
	drawCornersOpenPyramid(rightTopJet, _tid_blue);
	drawCornersOpenPyramid(leftBottomJet, _tid_blue);
	drawCornersOpenPyramid(rightBottomJet, _tid_blue);
	drawCornersOpenPyramid(topBody, ship2Tex);
	drawCornersPyramid(bottomBody, ship2Tex);
		/*Left Jet Flame*/
	drawExtQuad(-0.25f - ship2.throttle * 0.025f,-0.12f - ship2.throttle * 0.025f,0.6f + ship2.throttle * 0.05f, 0.1f + ship2.throttle * 0.05f,0.15f + ship2.throttle * 0.05f,0, (_tid_red * (((int)timer % 80) < 40)) + (_tid_yellow * (((int)timer % 80) >= 40)));
		/*Right Jet Flame*/
	drawExtQuad(0.15f - ship2.throttle * 0.025f,-0.12f - ship2.throttle * 0.025f,0.6f + ship2.throttle * 0.05f, 0.1f + ship2.throttle * 0.05f,0.15f + ship2.throttle * 0.05f,0, (_tid_red * (((int)timer % 80) < 40)) + (_tid_yellow * (((int)timer % 80) >= 40)));
}

//Crosshair, you draw it
void drawCrosshair(int i)
{
	glLoadIdentity();
	glTranslatef(crosshair[i].x,crosshair[i].y,crosshair[i].z);
	drawExtQuad(-0.2f,-0.2f,0.0f, 0.4f,0.4f,0.0f,_tid_crosshair);
}

//Cursor Crosshair, you draw it
void drawCursor(int i)
{
	glLoadIdentity();
	glTranslatef(cursor[i].x,cursor[i].y,cursor[i].z);
	drawExtQuad(-0.2f,-0.2f,0.0f,  0.4f,0.4f,0.0f,  _tid_outerCrosshair);
}

//draw fire
void drawFire()
{
	for(int i = 0;i < fire.maxFire;i++)
	{
		if(fire.shot[i].team != "Empty")
		{
			glLoadIdentity();
			glTranslatef(fire.shot[i].pos.x,fire.shot[i].pos.y,fire.shot[i].pos.z);
			glRotatef(fire.shot[i].direction.y * 180,1.0f,0.0f,0.0f);
			glRotatef(- fire.shot[i].direction.x * 180,0.0f,1.0f,0.0f);
			glRotatef( (GLfloat)(((int)timer % 60) * 3) , (GLfloat)0.0f , (GLfloat)0.0f , (GLfloat)1.0f );
			drawCornersPyramid(fireFront, _tid_red);
			drawCornersPyramid(fireBack, _tid_red);
		}
	}
}

//background, you draw it
void drawBackground()
{
	for(int i = 0;i < maxStars;i++) drawExtQuad(star[i].x,star[i].y,star[i].z, star[i].l,star[i].w,0, _tid_white);
}

//HP, you draw it
void drawHP()
{
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glColor3f(2.5f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	
	glNormal3f(1.0f, 1.0f, 1.0f);
	l = 1.2f * (ship.HP / ship.maxHP);	w = 0.04f;	h = 0.0f; x = -1.5f; y = 1.0f;	z = -3.0f;
	glVertex3f(x, y, z);
	glVertex3f(x + l, y, z);
	glVertex3f(x + l, y + w, z);
	glVertex3f(x, y + w, z);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	
	glNormal3f(1.0f, 0.0f, 0.0f);
	l = 1.2f;
	glVertex3f(x, y, z);
	glVertex3f(x + l, y, z);
	glVertex3f(x + l, y + w, z);
	glVertex3f(x, y + w, z);
	glEnd();

	glLoadIdentity();
}

//HP, you draw it2
void drawHP2()
{
	glLoadIdentity();

	glDisable(GL_TEXTURE_2D);
	glColor3f(2.5f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
	
	glNormal3f(1.0f, 1.0f, 1.0f);
	l = 1.2f * (ship2.HP / ship2.maxHP);	w = 0.04f;	h = 0.0f; x = 0.4f; y = 1.0f;	z = -3.0f;
	glVertex3f(x, y, z);
	glVertex3f(x + l, y, z);
	glVertex3f(x + l, y + w, z);
	glVertex3f(x, y + w, z);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	
	glNormal3f(1.0f, 0.0f, 0.0f);
	l = 1.2f;
	glVertex3f(x, y, z);
	glVertex3f(x + l, y, z);
	glVertex3f(x + l, y + w, z);
	glVertex3f(x, y + w, z);
	glEnd();

	glLoadIdentity();
}

//Enemy, you draw it
void drawEnemy()
{
	GLuint shipTex;
	for(int i = 0;i < enemy.maxEnemy;i++)
	{
		if(enemy.en[i].HP > 0)
		{
			shipTex = _tid_black;
			if(enemy.en[i].damageTime > 1 && (int)timer % 30 != 1)
			{
				shipTex = _tid_red;
			}
			glLoadIdentity();
			glTranslatef(enemy.en[i].pos.x,enemy.en[i].pos.y,enemy.en[i].pos.z);
			glRotatef(enemy.en[i].rotate.x,1.0f,0.0f,0.0f);
			glRotatef(enemy.en[i].rotate.y,0.0f,1.0f,0.0f);
			glRotatef(enemy.en[i].rotate.z,0.0f,0.0f,1.0f);
				/*Enemy*/
			drawCornersPyramid(enemyBody, shipTex);
			drawCornersPyramid(enemyJet, shipTex);
		}
	}
}

//Draws the new vertices of all shapes
void drawScene()
{
	initCamLight();


	drawBackground();
	drawFire();
	drawEnemy();
	
	if(ship.HP > 0)
	{
		drawHP();
		drawCursor(0);
		drawCrosshair(0);
	}

	if(ship2.HP > 0)
	{
		drawHP2();
		drawCursor(1);
		drawCrosshair(1);
	}

	drawArwing();
	drawArwing2();

	glutSwapBuffers();
}

//init all possible drawing items prior to actions
void initDrawItems()
{
	//ARWING
	leftWing.back =  newPoint(-0.5f , -0.1f , +0.5f );
	leftWing.right = newPoint(-0.1f , -0.1f , +0.4f );
	leftWing.front = newPoint(-0.2f , -0.1f , +0.4f );
	leftWing.left =  newPoint(-1.1f , -0.2f , +0.5f );
	leftWing.top =   newPoint(-0.5f , +0.0f , +0.3f );
	rightWing = invertPyramidCornersX(leftWing );
	leftTopJet.back =  newPoint(-0.2f , -0.1f , -0.0f );
	leftTopJet.right = newPoint(-0.1f , -0.1f , 0.6f );
	leftTopJet.front = newPoint(-0.2f , -0.1f , 0.5f );
	leftTopJet.left =  newPoint(-0.3f , -0.1f , 0.6f );
	leftTopJet.top =   newPoint(-0.2f , +0.2f , 0.7f );
	rightTopJet = invertPyramidCornersX(leftTopJet );
	leftBottomJet.back =  newPoint(-0.2f , -0.1f , -0.0f );
	leftBottomJet.right = newPoint(-0.1f , -0.1f , 0.6f );
	leftBottomJet.front = newPoint(-0.2f , -0.1f , 0.5f );
	leftBottomJet.left =  newPoint(-0.3f , -0.1f , 0.6f );
	leftBottomJet.top =   newPoint(-0.2f , -0.15f , 0.7f );
	rightBottomJet = invertPyramidCornersX(leftBottomJet );
	topBody.back =  newPoint(+0.0f , -0.1f , -0.1f );
	topBody.right = newPoint(+0.15f , -0.1f , 0.4f );
	topBody.front = newPoint(+0.0f , -0.1f , 0.5f );
	topBody.left =  newPoint(-0.15f , -0.1f , 0.4f );
	topBody.top =   newPoint(-0.0f , +0.1f , 0.6f );
	bottomBody.back =  newPoint(+0.0f , -0.15f , -0.7f );
	bottomBody.right = newPoint(+0.15f , -0.1f , 0.4f );
	bottomBody.front = newPoint(+0.0f , -0.2f , 0.6f );
	bottomBody.left =  newPoint(-0.15f , -0.1f , 0.4f );
	bottomBody.top =   newPoint(-0.0f , +0.0f , 0.5f );

	//FIRE
	fireFront.back =  newPoint(+0.0f , +0.2f , +0.0f );
	fireFront.right = newPoint(+0.2f , +0.0f , +0.0f );
	fireFront.front = newPoint(+0.0f , -0.2f , +0.0f );
	fireFront.left =  newPoint(-0.2f , +0.0f , +0.0f );
	fireFront.top =   newPoint(+0.0f , +0.0f , -0.2f );
	fireBack.back =  fireFront.back;
	fireBack.right = fireFront.right;
	fireBack.front = fireFront.front;
	fireBack.left =  fireFront.left;
	fireBack.top =    newPoint(+0.0f , +0.0f , +0.8f );

	//ENEMY
	enemyBody.back =  newPoint(+0.0f , +0.1f , +0.7f );
	enemyBody.right = newPoint(+0.7f , +0.1f , +0.0f );
	enemyBody.front = newPoint(+0.0f , +0.1f , -0.7f );
	enemyBody.left =  newPoint(-0.7f , +0.1f , +0.0f );
	enemyBody.top =   newPoint(-0.0f , -0.3f , +0.0f );
	enemyJet.back =  newPoint(-0.0f , -0.4f , -0.7f );
	enemyJet.right = newPoint(+0.2f , -0.0f , -0.7f );
	enemyJet.front = newPoint(-0.0f , -0.0f , -0.4f );
	enemyJet.left =  newPoint(-0.2f , -0.0f , -0.7f );
	enemyJet.top =   newPoint(-0.0f , +0.2f , -0.7f );
}