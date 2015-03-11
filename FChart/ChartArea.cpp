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

}

ChartArea::~ChartArea()
{
	delete pAxis;
	this->pPlatform->Release();
	this->pBrush->Release();
}
void ChartArea::SetRect(const Rect& rc)
{
	this->rcArea = rc;
	this->pAxis->SetRect(rc);
	
}
void ChartArea::SetBackground(const int32_t& color, bool drawBg)
{
	
}
void ChartArea::Draw()
{
	this->pPlatform->SetBrush(this->pBrush, BrushStyle::Fill);
	this->pPlatform->DrawRect(this->rcArea, BrushStyle::Fill);
	this->pAxis->Draw();
}


void ChartArea::OnMouseMove(const MouseEventArgs& e)
{
	
}