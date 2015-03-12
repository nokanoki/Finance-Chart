#include "ChartArea.h"

using namespace fchart;






ChartArea::ChartArea(IPlatform *pPlatform) :
pPlatform(pPlatform), pBrush(nullptr)
{
	this->pPlatform->AddRef();
	this->pBrush = this->pPlatform->CreateBrush(makesolidbrushprps(0xff0f0f0f));
	this->transformation = maketransformation();
}
ChartArea::~ChartArea()
{
	for (auto axis : this->axies)
		axis->Release();
	for (auto s : this->series)
		s->Release();

	this->pPlatform->Release();
	this->pBrush->Release();
}
void ChartArea::SetRect(const Rect& rc)
{
	this->rcArea = rc;
	for (auto axis : this->axies)
		axis->SetRect(rc);
	for (auto s : this->series)
		s->SetRect(rc);
	
}
void ChartArea::SetBackground(const int32_t& color, bool drawBg)
{
	
}
void ChartArea::Draw()
{
	this->pPlatform->SetBrush(this->pBrush, BrushStyle::Fill);
	this->pPlatform->DrawRect(this->rcArea, BrushStyle::Fill);

	for (auto axis : this->axies)
		axis->Draw();
	for (auto s : this->series)
		s->Draw();
	
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
		axis->SetTransformation(transformation);
	for (auto s : this->series)
		s->SetTransformation(transformation);
}

IAxis* ChartArea::CreateAxis(const AxisType& type)
{
	return new Axis(this->pPlatform, type);
}
void ChartArea::AddAxis(IAxis* pAxis)
{
	this->axies.push_back(dynamic_cast<Axis*>(pAxis));
	pAxis->AddRef();
}
ISeries* ChartArea::CreateSeries()
{
	return new Series(this->pPlatform);
}
//rename that
void ChartArea::SetSeries(ISeries *pSeries)
{
	this->series.push_back(dynamic_cast<Series*>(pSeries));
	pSeries->AddRef();
	for (auto a : this->axies)
		a->SetData(dynamic_cast<Series*>(pSeries)->GetData());
}
void ChartArea::AddData(const Quotation* pData, const int32_t& count)
{
	std::copy(pData, pData + count, std::back_inserter(this->data));


}