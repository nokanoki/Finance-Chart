#include "ChartArea.h"

using namespace fchart;






ChartArea::ChartArea(IPlatform *pPlatform) :
pPlatform(pPlatform), pBrush(nullptr)
{
	this->pPlatform->AddRef();
	this->pBrush = this->pPlatform->CreateBrush(makesolidbrushprps(0xff0f0f0f));
	this->transformation = maketransformation();
	this->CreateAxis(L"default x", AxisType::Horizontal);
	this->CreateAxis(L"default y", AxisType::Vertical);
	
}
ChartArea::~ChartArea()
{
	for (auto axis : this->axies)
		axis.second->Release();
	for (auto s : this->series)
		s.second->Release();

	this->pPlatform->Release();
	this->pBrush->Release();
}
IChartArea* ChartArea::SetRect(const Rect& rc)
{
	this->rcArea = rc;
	Rect rcSeries = rc;
	for (auto axis : this->axies)
	{
		if (axis.second->GetAxisType() == AxisType::Vertical)
			rcSeries.right -= axis.second->GetAxisSize();
		else
			rcSeries.bottom += axis.second->GetAxisSize();
		axis.second->SetRect(rcSeries);
	}
	
	for (auto s : this->series)
		s.second->SetRect(rcSeries);
	return this;
}
void ChartArea::SetBackground(const int32_t& color, bool drawBg)
{
	
}
void ChartArea::Draw()
{
	this->pPlatform->SetBrush(this->pBrush, BrushStyle::Fill);
	this->pPlatform->DrawRect(this->rcArea, BrushStyle::Fill);

	for (auto axis : this->axies)
		axis.second->Draw();
	for (auto s : this->series)
		s.second->Draw();
	
}


void ChartArea::OnMouseMove(const MouseEventArgs& e)
{
	//is in area?
	if (e.x < this->rcArea.left
		|| e.x > this->rcArea.right
		|| e.y > this->rcArea.top
		|| e.y < this->rcArea.bottom)
		return;
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
		transformation.tx += mouse.x - mouse.xlast;
		transformation.ty += mouse.y - mouse.ylast;
	}

	this->transformation.sy = 100.f;

	
	for (auto axis : this->axies)
		axis.second->SetTransformation(transformation);
	for (auto s : this->series)
		s.second->SetTransformation(transformation);
}

IAxis* ChartArea::CreateAxis(const wchar_t* name,const AxisType& type)
{
	auto axis = new Axis(this->pPlatform, type,this);
	this->axies[name] = axis;
	return axis;
}
IAxis* ChartArea::GetAxis(const wchar_t* name)
{
	return this->axies[name];
}
ISeries* ChartArea::CreateSeries(const wchar_t* name)
{
	auto s = new Series(this->pPlatform);
	s->SetRect(this->rcArea);
	this->series[name] = s;
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