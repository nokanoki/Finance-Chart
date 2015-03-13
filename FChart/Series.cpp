#include "Series.h"
#include "ChartArea.h"
#include <algorithm>
using namespace fchart;



Series::Series(IPlatform *pPlatform, ChartArea *pArea) :
pPlatform(pPlatform), pChartArea(pArea)
{
	pPlatform->AddRef();
	this->pBrushGreen = this->pPlatform->CreateBrush(makesolidbrushprps(0xff00ff00));
	this->pBrushRed = this->pPlatform->CreateBrush(makesolidbrushprps(0xffFF0000));
	this->pBrushWhite = this->pPlatform->CreateBrush(makesolidbrushprps(0xffFFffFF));
	this->transformation = maketransformation();
	this->seriesType = SeriesType::Line;
	this->seriesFocus = SeriesFocus::Auto;
	this->dataPointWidth = 10.f; //ref candlestick draw
}
Series::~Series()
{
	this->pPlatform->Release();
	this->pBrushGreen->Release();
	this->pBrushRed->Release();
	this->pBrushWhite->Release();
}

void Series::SetRect(const Rect& rc)
{
	this->rcSeries = rc;
}
void Series::SetTransformation(const Transformation& trans)
{
	this->transformation = trans;
}
ISeries* Series::SetSeriesType(const SeriesType& type)
{
	this->seriesType = type;
	return this;
}
ISeries* Series::AddData(const Quotation *pData, const int32_t& count)
{
	std::copy(pData, pData + count, std::back_inserter(this->data));
	if (this->seriesFocus == SeriesFocus::Auto)
	{
		//calc transformation
		Transformation tr = maketransformation();
		float halfWidth = (this->rcSeries.right - this->rcSeries.left) / 2.f;

		int32_t dataPointCount = static_cast<int32_t>(halfWidth / this->dataPointWidth);
		if (dataPointCount <= this->data.size())
		{
			//tr.tx = dataPointCount - this->data.size();
			float maxy = std::numeric_limits<float>::min();
			float miny = std::numeric_limits<float>::max();
			for (size_t i = this->data.size() - dataPointCount; i < this->data.size(); i++)
			{
				maxy = std::max(this->data[i].high, maxy);
				miny = std::min(this->data[i].low, miny);
			}
			tr.sy = (this->rcSeries.top - this->rcSeries.bottom) / ((maxy - miny) * 2.f);
		
			tr.ty = -1.f * ((miny * tr.sy) / 2.f);
			tr.tx = -((this->data.size() - dataPointCount) * this->dataPointWidth);
			//normalize
			tr.sy = static_cast<float>(static_cast<int>(tr.sy));
			tr.ty = static_cast<float>(static_cast<int>(tr.ty));
			

		}
		this->pChartArea->SetTransformation(tr);
	}
	return this;
}
std::vector<Quotation> Series::GetData()
{
	return this->data;
}
void Series::Draw()
{
	int i = 0;
	if (this->seriesType == SeriesType::Candlestick)
	{
		for (auto q : this->data)
		{

			float y[] = {
				q.high * transformation.sy + transformation.ty,
				q.low *transformation.sy + transformation.ty,
				q.open * transformation.sy + transformation.ty,
				q.close * transformation.sy + transformation.ty };
			float x = (i += 10) * transformation.sx + transformation.tx;

			//check overflow
			if (x > this->rcSeries.right)
				break;//nothing else to draw
			if (x < this->rcSeries.left)
				continue;//skip that
			if (y[1] > this->rcSeries.top || y[0] < this->rcSeries.bottom)//candle isnt visible
				continue; //skip that


			this->pPlatform->SetBrush(this->pBrushWhite, BrushStyle::Outline);
			//is up
			if (y[2] <= y[3])
			{
				this->pPlatform->SetBrush(this->pBrushGreen, BrushStyle::Fill);
				std::swap(y[2], y[3]);
			}
			else
			{
				this->pPlatform->SetBrush(this->pBrushRed, BrushStyle::Fill);
			}

			//check candles body overflow
			if (y[0] > this->rcSeries.top) y[0] = this->rcSeries.top;
			if (y[1] > this->rcSeries.top) y[1] = this->rcSeries.top;
			if (y[2] > this->rcSeries.top) y[2] = this->rcSeries.top;
			if (y[3] > this->rcSeries.top) y[3] = this->rcSeries.top;

			if (y[0] < this->rcSeries.bottom) y[0] = this->rcSeries.bottom;
			if (y[1] < this->rcSeries.bottom) y[1] = this->rcSeries.bottom;
			if (y[2] < this->rcSeries.bottom) y[2] = this->rcSeries.bottom;
			if (y[3] < this->rcSeries.bottom) y[3] = this->rcSeries.bottom;

			this->pPlatform->DrawCandlestick(y, x);
		}
	}
	else if (this->seriesType == SeriesType::Line)
	{
		this->pPlatform->SetBrush(this->pBrushWhite, BrushStyle::Outline);
		
		bool isFirstSet = false;
		float xlast , ylast;
		float i = 0;
		for (auto q : this->data)
		{
			if (!isFirstSet)
			{
				ylast = q.high * this->transformation.sy + this->transformation.ty;
				xlast = (i += 10) * transformation.sx + transformation.tx;
				isFirstSet = true;
				continue;
			}
			float y = q.high * this->transformation.sy + this->transformation.ty;
			float x = (i += 10) * transformation.sx + transformation.tx;

			//check left right ofverflows
			if (x > this->rcSeries.right
				|| x < this->rcSeries.left
				|| xlast > this->rcSeries.right
				|| xlast < this->rcSeries.left)
			{
				xlast = x;
				ylast = y;
				continue;
			}
			//check top bottom overflows
			/*if (y > this->rcSeries.top
				|| y < this->rcSeries.bottom
				|| ylast > this->rcSeries.top
				|| ylast < this->rcSeries.bottom)
			{
				isFirstSet = false;
				continue;
			}*/
			if (y > this->rcSeries.top) y = this->rcSeries.top;
			if (y < this->rcSeries.bottom) y = this->rcSeries.bottom;
			if (ylast > this->rcSeries.top) ylast = this->rcSeries.top;
			if (ylast < this->rcSeries.bottom) ylast = this->rcSeries.bottom;

			this->pPlatform->DrawLine(makepoint(xlast, ylast), makepoint(x, y));
			xlast = x;
			ylast = y;
		}
	}
	else if (this->seriesType == SeriesType::Band)
	{
		this->pPlatform->SetBrush(this->pBrushWhite, BrushStyle::Outline);

		bool isFirstSet = false;
		float xlast, ylast0, ylast1;
		float i = 0;
		for (auto q : this->data)
		{
			if (!isFirstSet)
			{
				ylast0 = q.high * this->transformation.sy + this->transformation.ty;
				ylast1 = q.low * this->transformation.sy + this->transformation.ty;
				xlast = (i += 10) * transformation.sx + transformation.tx;
				isFirstSet = true;
				continue;
			}
			float y0 = q.high * this->transformation.sy + this->transformation.ty;
			float y1 = q.low * this->transformation.sy + this->transformation.ty;
			float x = (i += 10) * transformation.sx + transformation.tx;

			//check left right ofverflows
			if (x > this->rcSeries.right
				|| x < this->rcSeries.left
				|| xlast > this->rcSeries.right
				|| xlast < this->rcSeries.left)
			{
				xlast = x;
				ylast0 = y0;
				ylast1 = y1;
				continue;
			}
			
			if (y0 > this->rcSeries.top) y0 = this->rcSeries.top;
			if (y0 < this->rcSeries.bottom) y0 = this->rcSeries.bottom;
			if (y1 > this->rcSeries.top) y1 = this->rcSeries.top;
			if (y1 < this->rcSeries.bottom) y1 = this->rcSeries.bottom;
			if (ylast0 > this->rcSeries.top) ylast0 = this->rcSeries.top;
			if (ylast0 < this->rcSeries.bottom) ylast0 = this->rcSeries.bottom;
			if (ylast1 > this->rcSeries.top) ylast1 = this->rcSeries.top;
			if (ylast1 < this->rcSeries.bottom) ylast1 = this->rcSeries.bottom;

			this->pPlatform->DrawLine(makepoint(xlast, ylast0), makepoint(x, y0));
			this->pPlatform->DrawLine(makepoint(xlast, ylast1), makepoint(x, y1));
			xlast = x;
			ylast0 = y0;
			ylast1 = y1;
		}
	}
}
