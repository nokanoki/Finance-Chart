#include "ChartArea.h"

using namespace fchart;






ChartArea::ChartArea(IPlatform *pPlatform) :
pPlatform(pPlatform), pBrush(nullptr)
{
	this->pPlatform->AddRef();
	this->pBrush = this->pPlatform->CreateBrush(makesolidbrushprps(0xff000000));
	AxisProperties props;
	props.type = AxisType::Vertical; 
	props.position = AxisPosition::Right;
	props.lblFactor = 20.f;
	props.gridFactor = 40.f;
	this->pAxis = new Axis(pPlatform,props);

	Quotation q[] = 
	{
#include "testquotes.array"
	};
	this->pSeries = new Series(this->pPlatform);
	this->pSeries->AddData(q, _countof(q));

	this->transformation.sx = 1.f;
	this->transformation.sy = 1.f;
	this->transformation.tx = 0.f;
	this->transformation.ty = 0.f;
}

ChartArea::~ChartArea()
{
	delete pAxis;
	delete pSeries;
	this->pPlatform->Release();
	this->pBrush->Release();
}
void ChartArea::SetRect(const Rect& rc)
{
	this->rcArea = rc;
	this->pAxis->SetRect(rc);
	this->pSeries->SetRect(rc);
	
}
void ChartArea::SetBackground(const int32_t& color, bool drawBg)
{
	
}
void ChartArea::Draw()
{
	this->pPlatform->SetBrush(this->pBrush, BrushStyle::Fill);
	this->pPlatform->DrawRect(this->rcArea, BrushStyle::Fill);

	
	this->pSeries->Draw();
	this->pAxis->Draw();
}


void ChartArea::OnMouseMove(const MouseEventArgs& e)
{
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

	this->pSeries->SetTransformation(transformation);
	this->pAxis->SetTransformation(transformation);
}