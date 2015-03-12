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

}