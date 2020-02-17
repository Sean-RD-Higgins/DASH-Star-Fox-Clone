/*Programmer: Sean Higgins 
*/
#ifndef STRUCTS_H
#define STRUCTS_H

//Structures
struct Point
{
	float x;
	float y;
	float z;
};
struct PyramidCorners
{
	Point left;
	Point right;
	Point front;
	Point back;
	Point top;
};
struct BoxCorners
{
	Point FrontLeft;
	Point FrontRight;
	Point FrontTop;
	Point FrontBottom;
	Point BackLeft;
	Point BackRight;
	Point BackTop;
	Point BackBottom;
};
struct Box2Corners
{
	Point FrontTopLeft;
	Point BackBottomRight;
};
struct Star
{
	float x;
	float y;
	float z;
	float l;
	float w;
};
struct HitBox
{
	Box2Corners box;
	char * team;
};
struct BoundBox
{
	float Left;
	float Right;
	float Top;
	float Bottom;	
	float Front;
	float Back;
};
#endif
