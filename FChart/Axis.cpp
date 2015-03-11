#include "Axis.h"
using namespace fchart;

Axis::Axis(IPlatform *pPlatform, const AxisProperties& props) :
pPlatform(pPlatform), properties(props)
{
	pPlatform->AddRef();
	this->pTextBrush = pPlatform->CreateBrush(makesolidbrushprps(0xafFFffFF));
	this->pTextFormat = pPlatform->CreateTextFormat(maketextprps());
}
Axis::~Axis()
{
	this->pTextBrush->Release();
	this->pTextFormat->Release();
	pPlatform->Release();
	
}

void Axis::SetRect(const Rect& rc)
{
	this->rcAxis = rc;
	
}




void Axis::Draw()
{
	if (this->properties.type == AxisType::Vertical)
	{
		this->DrawVertical();
		
	}
	
}
void Axis::DrawVertical()
{

	float top, bottom, left, right;

	switch (this->properties.position)
	{
	case AxisPosition::Right:
		left = this->rcAxis.right;
		right = this->rcAxis.right + 100.f;
		break;
	case AxisPosition::Left:
		left = this->rcAxis.left -100.f;
		right = this->rcAxis.left;
		break;
	default:
		break;
	}
	top = this->rcAxis.top;
	bottom = this->rcAxis.bottom;
	
	float heigh = top - bottom;
	float count = heigh / this->properties.lblFactor;
	//remove overflow
	top -= this->properties.lblFactor / 2.f; 
	bottom += this->properties.lblFactor / 2.f;;


	this->pPlatform->SetTextFormat(this->pTextFormat);
	this->pPlatform->SetBrush(this->pTextBrush, BrushStyle::Fill);

	
	for (float i = bottom; i < top; i += this->properties.lblFactor)
	{
		pPlatform->DrawText(makepoint(left, i + this->properties.lblFactor/2.f), std::to_wstring(i).c_str());
	}

	//draw grid
	for (float i = bottom; i < top; i += this->properties.gridFactor)
	{
		pPlatform->DrawLine(
			makepoint(this->rcAxis.left, i),
			makepoint(this->rcAxis.right, i),
			StrokeStyle::Dash);
	}
}