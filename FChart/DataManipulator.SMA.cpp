#include "DataManipulator.SMA.h"

using namespace fchart::DataManipulator;
using namespace fchart;

SMA::SMA(const int32_t& val) : val(val)
{

}

void SMA::Calc(const Quotation* in, Quotation* out, const size_t& count)
{
	size_t sma = 40;
	if (sma > count)
		return;
	for (size_t i = 0; i < sma; i++)
	{
		out[i].high = 0.f;
		out[i].low = 0.f;
		out[i].open = 0.f;
		out[i].close = 0.f;
		out[i].volume = 0;
		out[i].time = 0;
	}
	for (size_t i = sma; i < count; i++)
	{
		float avg = 0.f;
		for (size_t j = i - sma; j < i; j++)
			avg += in[j].close;
		out[i].close = avg / float(sma);
	}
}
