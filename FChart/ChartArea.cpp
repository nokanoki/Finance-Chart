#include "ChartArea.h"
#include "Chart.h"
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>
using namespace fchart;





ChartArea::ChartArea(IPlatform *pPlatform, Chart *pChart) :
pPlatform(pPlatform),pChart(pChart) ,pBrush(nullptr)
{
	this->pPlatform->AddRef();
	this->pBrush = this->pPlatform->CreateBrush(makesolidbrushprps(0xff0f0f0f));
	this->transformation = maketransformation();
	this->isXAxisSync = false;
	this->pTextFormat = this->pPlatform->CreateTextFormat(maketextprps());
	this->pAxisYTextBrush = this->pPlatform->CreateBrush(makesolidbrushprps(0xffFFffFF));

}
ChartArea::~ChartArea()
{

	this->pTextFormat->Release();
	this->pAxisYTextBrush->Release();

	for (auto s : this->series)
		delete s.second;
	

	this->pPlatform->Release();
	this->pBrush->Release();
}
IChartArea* ChartArea::SetRect(const Rect& rc)
{
	this->rcArea = rc;
	this->rcSeries = rc;



	this->rcSeries.right -= 100.f;
	this->rcSeries.bottom += 30.f;
	
	this->rcAxisY = rc;
	this->rcAxisY.left = this->rcAxisY.right - 100.f;
	

	
	
	for (auto s : this->series)
		s.second->SetRect(rcSeries);
	return this;
}
void ChartArea::SetBackground(const int32_t& color, bool drawBg)
{
	
}
void ChartArea::Draw(const std::map <std::wstring, Buffer>& buffers)
{
	this->pPlatform->SetBrush(this->pBrush, BrushStyle::Fill);
	this->pPlatform->DrawRect(this->rcArea, BrushStyle::Fill);

	for (auto s : this->series)
		s.second->Draw(buffers.find(s.second->GetBufferSourceName())->second.data);
	
	this->DrawAxisVertical();

	
}



void ChartArea::OnMouseMove(const MouseEventArgs& e)
{
	//is in area?
	
	if (e.x < this->rcArea.left
		|| e.x > this->rcArea.right
		|| e.y > this->rcArea.top
		|| e.y < this->rcArea.bottom)
		return;//nope skip it
	//calc dragging
	if (e.buttonState.left)
	{
		if (!mouse.isDragging)
		{
			mouse.isDragging = true;
			mouse.x = mouse.xstart = mouse.xlast = e.x;
			mouse.y = mouse.ystart = mouse.ylast = e.y;
		}
		else
		{
			mouse.xlast = mouse.x;
			mouse.ylast = mouse.y;
			mouse.x = e.x;
			mouse.y = e.y;
		}
	}
	else
		mouse.isDragging = false;

	if (mouse.isDragging)
	{
		//is in series
		if (!this->series.empty())
		{
			if (!(e.x  < this->rcSeries.left
				|| e.x > this->rcSeries.right
				|| e.y > this->rcSeries.top
				|| e.y < this->rcSeries.bottom))
			{
				this->transformation.tx -= mouse.xlast - mouse.x;
				this->transformation.ty -= mouse.ylast - mouse.y;
			}
			else//check axies
			{
#if 0
				for (auto a : this->axies)
				{
					if (( e.x > a.second->GetLabelRect().left
						&& e.x < a.second->GetLabelRect().right
						&& e.y < a.second->GetLabelRect().top
						&& e.y > a.second->GetLabelRect().bottom))
					{
						if (a.second->GetAxisType() == AxisType::Vertical)
						{
							if ((mouse.xlast - mouse.x) != 0.f)
								transformation.sy += (mouse.ylast - mouse.y) / 1.f;
						}
						else
							if ((mouse.xlast - mouse.x) != 0.f)
								transformation.sx += (mouse.xlast - mouse.x) / 1.f;
					}
				}
#endif
			}
		}
	}

	for (auto s : this->series)
		s.second->SetTransformation(transformation);

	if (this->isXAxisSync)
	{
		for (auto c : this->pChart->GetChartAreas())
		{
			if (c.second != this && c.second->IsXAxisSync())
			{
				auto tr = c.second->GetTransformation();
				tr.tx = transformation.tx;
				tr.ty = transformation.ty;
				c.second->SetTransformation(tr);
			}
		}
	}
}


ISeries* ChartArea::CreateSeries(const wchar_t* name)
{
	auto s = new Series(this->pPlatform,this);
	this->series[name] = s;
	SetRect(this->rcArea);
	return s;
}
ISeries* ChartArea::GetSeries(const wchar_t* name)
{
	return this->series[name];
}

const Rect& ChartArea::GetRect()
{
	return this->rcArea;
}
void ChartArea::SetTransformation(const Transformation& trans)
{
	this->transformation = trans;
	
	for (auto s : this->series)
		s.second->SetTransformation(transformation);
}
const Transformation& ChartArea::GetTransformation()
{
	return this->transformation;
}
IChartArea* ChartArea::SetXAxisSync(const bool& onOff)
{
	this->isXAxisSync = onOff;
	return this;
}
bool ChartArea::IsXAxisSync()
{
	return this->isXAxisSync;
}


IChartArea* ChartArea::FocusLast(const wchar_t* seriesName)
{
	auto s = this->series[seriesName];
	
	auto d = this->pChart->GetData(s->GetBufferSourceName());
	
	auto l = [](const Quotation& q0,const Quotation& q1)
	{
		return q0.close < q1.close;
	};

	//calc last datapoints
	size_t maxDataPoints = static_cast<size_t>((this->rcSeries.right - this->rcSeries.left) / 10.f);
	maxDataPoints /= 2;
	auto first = d.begin();
	if (maxDataPoints < d.size())
		first = d.end() - maxDataPoints;

	auto minmax = std::minmax_element(first, d.end(), l);
	
	this->transformation.sy = (this->rcSeries.top - this->rcSeries.bottom) / (minmax.second->close - minmax.first->close);
	this->transformation.tx = -(std::distance(d.begin(), first) * 10.f);
	this->transformation.ty = -(this->transformation.sy * minmax.first->close);
	this->pChart->Render();
	return this;
}

IChartArea* ChartArea::SetYBoundsTest(const float& max, const float& min)
{
	this->transformation.sy = (this->rcSeries.top - this->rcSeries.bottom) / (max - min);
	this->transformation.ty = min;
	return this;
}


void ChartArea::DrawAxisVertical()
{
	this->pPlatform->SetTextFormat(this->pTextFormat);
	this->pPlatform->SetBrush(this->pAxisYTextBrush, BrushStyle::Fill);



	TextFormatProperties prps;
	this->pTextFormat->GetProperties(prps);
	//http://stackoverflow.com/questions/326679/choosing-an-attractive-linear-scale-for-a-graphs-y-axis
	float maxLabels = (this->rcAxisY.top - this->rcAxisY.bottom) / (prps.fontSize * 1.2f);
	float start = (this->rcAxisY.bottom - this->transformation.ty) / this->transformation.sy;
	float end = (this->rcAxisY.top - this->transformation.ty) / this->transformation.sy;
	float step = (end - start) / maxLabels;

	float magPow = std::powf(10.f, std::floorf(std::log10f(step)));
	float magMsg = roundf(step / magPow + 0.5f);
	step = magMsg * magPow;
	start = step * std::ceilf(start / step);
	end = step * std::floorf(end / step);



	for (float i = start; i < end; i += step)
	{
		std::wstringstream ss;
		ss << std::fixed << std::setprecision(5) << i;
		float y = i * transformation.sy + transformation.ty;
		y += (prps.fontSize * 1.f) / 2.f;
		pPlatform->DrawText(
			makepoint(this->rcAxisY.right -100.f /*this->axisSize */, y),
			ss.str().c_str());
	}
}