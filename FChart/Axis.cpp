#include "Axis.h"
#include <ctime>
using namespace fchart;



Axis::Axis(IPlatform *pPlatform, const AxisType& type) :
pPlatform(pPlatform), axisType(type)
{
	pPlatform->AddRef();
	this->pTextBrush = pPlatform->CreateBrush(makesolidbrushprps(0xafFFffFF));
	this->pTextFormat = pPlatform->CreateTextFormat(maketextprps());
	this->transformation = maketransformation();
	this->axisPosition = this->axisType == AxisType::Vertical ? AxisPosition::Right : AxisPosition::Bottom;
	this->lblFactor = this->axisType == AxisType::Vertical ? 20.f : 70.f;
	this->gridFactor = 20.f;
	this->dataPointWidth = 10.f;//ref Series::Draw candlestick
	this->axisDataType = AxisDataType::Price;
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
void Axis::SetTransformation(const Transformation& trans)
{
	this->transformation = trans;
}



void Axis::Draw()
{
	//todo merge that
	if (this->axisType == AxisType::Vertical)
		this->DrawVertical();
	else
		this->DrawHorizontal();
	
}
void Axis::DrawVertical()
{
	float top, bottom, left, right;
	switch (this->axisPosition)
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
	float count = heigh / this->lblFactor;
	//remove overflow
	top -= this->lblFactor / 2.f; 
	bottom += this->lblFactor / 2.f;;

	this->pPlatform->SetTextFormat(this->pTextFormat);
	this->pPlatform->SetBrush(this->pTextBrush, BrushStyle::Fill);
	
	for (float i = bottom; i < top; i += this->lblFactor)
		pPlatform->DrawText(makepoint(left, i + this->lblFactor / 2.f), std::to_wstring((i - transformation.ty) / transformation.sy).c_str());

	//draw grid
	for (float i = bottom; i < top; i += this->gridFactor)
	{
		pPlatform->DrawLine(makepoint(this->rcAxis.left, i), makepoint(this->rcAxis.right, i), StrokeStyle::Solid);
	}
}
void Axis::DrawHorizontal()
{
	float top, bottom, left, right;
	switch (this->axisPosition)
	{
	case AxisPosition::Bottom:
		top = this->rcAxis.bottom;
		bottom = top - 20.f;
		break;
	case AxisPosition::Top:
		bottom = this->rcAxis.top;
		top = bottom + 20.f;
		break;
	default:
		break;
	}
	left = this->rcAxis.left;
	right = this->rcAxis.right;

	float width = right - left;
	float count = width / this->lblFactor;

	this->pPlatform->SetTextFormat(this->pTextFormat);
	this->pPlatform->SetBrush(this->pTextBrush, BrushStyle::Fill);

	for (float i = left; i < width; i += this->lblFactor)
	{
		std::wstring lbl;
		switch (this->axisDataType)
		{
		case AxisDataType::Price:
			lbl = std::to_wstring(i * transformation.sx + transformation.tx);
			break;
		case AxisDataType::Date:
		{
			size_t idx = static_cast<size_t>(((i - this->transformation.tx) /this->transformation.sx)  / this->dataPointWidth);
			if (idx <= 0 || idx >= this->data.size())
				break;
			wchar_t buf[256] = { 0 };
			tm t;
			gmtime_s(&t, &this->data[idx].time);
			std::wcsftime(buf, 256, L"%x", &t);
			lbl = buf;
		}
			break;
		case AxisDataType::Time:
		{
			size_t idx = static_cast<size_t>(((i - this->transformation.tx) / this->transformation.sx) / this->dataPointWidth);
			if (idx <= 0 || idx >= this->data.size())
				break;
			wchar_t buf[256] = { 0 };
			tm t;
			gmtime_s(&t, &this->data[idx].time);
			std::wcsftime(buf, 256, L"%X", &t);
			lbl = buf;
		}
		break;
		default:
			lbl = L"Error";
		}
		pPlatform->DrawText(makepoint(i, bottom), lbl.c_str());
	}
}

void Axis::SetDataType(const AxisDataType& type)
{
	this->axisDataType = type;
}
void Axis::SetData(const std::vector<Quotation>& data)
{
	this->data = data;
}