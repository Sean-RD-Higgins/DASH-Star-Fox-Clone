#ifndef STRUCTS_H
#define STRUCTS_H
//Structures
struct frameStruct
{
	const float Left;
	const float Right;
	const float Top;
	const float Bottom;	
}Frame = {-3.2f, 2.9f, 0.0f, -4.4f};
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
struct Star
{
	float x;
	float y;
	float z;
	float l;
	float w;
	float h;
}star[30] = {0,0,0,0,0,0};
#endif
