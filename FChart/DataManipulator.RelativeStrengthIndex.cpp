#include "DataManipulator.RelativeStrengthIndex.h"


using namespace fchart;
using namespace fchart::DataManipulator;

/*                 100
    RSI = 100 - --------
                 1 + RS

    RS = Average Gain / Average Loss
	
	The very first calculations for average gain and average loss are simple 14 period averages.

	First Average Gain = Sum of Gains over the past 14 periods / 14.
	First Average Loss = Sum of Losses over the past 14 periods / 14
	The second, and subsequent, calculations are based on the prior averages and the current gain loss:

	Average Gain = [(previous Average Gain) x 13 + current Gain] / 14.
	Average Loss = [(previous Average Loss) x 13 + current Loss] / 14.
	*/

void RelativeStrengthIndex::Calc(const Quotation* in, Quotation* out, const size_t& count)
{
	int32_t period = 14;//todo export api

	if (count < 14)
		return;

	for (int32_t i = 0; i < period; i++)
	{
		out[i].high = 0.f;
		out[i].low = 0.f;
		out[i].open = 0.f;
		out[i].close = 0.f;
		out[i].volume = 0;
		out[i].time = 0;
	}

	int32_t i = period;
	float gain = 0.f, loss = 0.f;
	for (int32_t j = i - period ; j < i + 1; j++)
	{
		float r = in[j].close - in[j + 1].close;
		if (r < 0.f)
			gain += r;
		else
			loss += -r;
	}
	float rs = (gain / float(period)) / (loss / float(period));
	out[i].close = 100.f - (100.f / (1 + rs));
	out[i].time = in[i].time;

	
	for (i++; i < count; i++)
	{
		float r = in[i - 1].close - in[i].close;
		if (r < 0.f)
			gain = (gain * float(period - 1) + r) / float(period);
		else
			loss = (loss * float(period - 1) - r) / float(period);
		rs = (gain / float(period)) / (loss / float(period));
		out[i].close = 100.f - (100.f / (1.f + rs));
		out[i].time = in[i].time;
	}

}