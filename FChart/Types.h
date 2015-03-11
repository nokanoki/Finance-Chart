#pragma once
#include <time.h>
namespace fchart
{
	struct Quotation
	{
		time_t time;
		float high, low, open, close;
		int volume;
	};

	struct Size
	{
		float height;
		float width;
		__inline Size(float heigth, float width) : height(height), width(width){}
	};
	struct Position
	{
		float x;
		float y;
		__inline Position(float x, float y) : x(x), y(y){}
	};
	struct Point
	{
		float x;
		float y;
	};
}