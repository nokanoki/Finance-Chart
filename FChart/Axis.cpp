#include "Axis.h"
#include "ChartArea.h"
#include <ctime>
using namespace fchart;



Axis::Axis(IPlatform *pPlatform, const AxisType& type, ChartArea *pChartArea) :
pPlatform(pPlatform), axisType(type), chartArea(pChartArea)
{
	pPlatform->AddRef();
	this->pTextBrush = pPlatform->CreateBrush(makesolidbrushprps(0x8fFFffFF));
	this->pTextFormat = pPlatform->CreateTextFormat(maketextprps());
	this->transformation = maketransformation();
	this->axisPosition = this->axisType == AxisType::Vertical ? AxisPosition::Right : AxisPosition::Bottom;
	this->lblFactor = this->axisType == AxisType::Vertical ? 20.f : 70.f;
	this->gridFactor = 20.f;
	this->dataPointWidth = 10.f;//ref Series::Draw candlestick
	this->axisDataType = AxisDataType::Price;
	this->axisSize = this->axisType == AxisType::Vertical ? 100.f : 16.f;
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
	this->rcLabel = rc;
	if (this->axisType == AxisType::Horizontal)
	{
		if (this->axisPosition == AxisPosition::Bottom)
			rcLabel.top = rcLabel.bottom + this->axisSize;
		else if (this->axisPosition == AxisPosition::Top)
			rcLabel.bottom = rcLabel.top - this->axisSize;
	}
	else if (this->axisType == AxisType::Vertical)
	{
		if (this->axisPosition == AxisPosition::Right)
			rcLabel.left = rcLabel.right - this->axisSize;
		else if (this->axisPosition == AxisPosition::Left)
			rcLabel.right = rcLabel.left + this->axisSize;
	}
}
const Rect& Axis::GetLabelRect()
{
	return this->rcLabel;
}
IAxis* Axis::SetBufferSource(const wchar_t* name)
{
	this->bufferName = name;
	return this;
}
void Axis::SetTransformation(const Transformation& trans)
{
	this->transformation = trans;
}



void Axis::Draw(const std::vector<Quotation>& data)
{
	//todo merge that
	if (this->axisType == AxisType::Vertical)
		this->DrawVertical(data);
	else
		this->DrawHorizontal(data);
	
}
void Axis::DrawVertical(const std::vector<Quotation>& data)
{
	float top, bottom, left, right;
	switch (this->axisPosition)
	{
	case AxisPosition::Right:
		left = this->rcAxis.right;
		right = this->rcAxis.right + this->axisSize;
		break;
	case AxisPosition::Left:
		left = this->rcAxis.left - this->axisSize;
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
	{
		
		float price = (i - transformation.ty) / transformation.sy;
		price = static_cast<float>(static_cast<int>(price * 1000.f)) / 1000.f;
		pPlatform->DrawText(makepoint(left, i + this->lblFactor / 2.f), std::to_wstring(price).c_str());
	}
	//draw grid
	for (float i = bottom; i < top; i += this->gridFactor)
	{
		pPlatform->DrawLine(makepoint(this->rcAxis.left, i), makepoint(this->rcAxis.right, i), StrokeStyle::Solid);
	}
}
void Axis::DrawHorizontal(const std::vector<Quotation>& data)
{
	float top, bottom, left, right;
	switch (this->axisPosition)
	{
	case AxisPosition::Bottom:
		top = this->rcAxis.bottom;
		bottom = top;// +this->axisSize; ??? wtf?
		break;
	case AxisPosition::Top:
		bottom = this->rcAxis.top;
		top = bottom + this->axisSize;
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
			if (idx <= 0 || idx >= data.size())
				break;
			wchar_t buf[256] = { 0 };
			tm t;
			gmtime_s(&t, &data[idx].time);
			std::wcsftime(buf, 256, L"%x", &t);
			lbl = buf;
		}
			break;
		case AxisDataType::Time:
		{
			size_t idx = static_cast<size_t>(((i - this->transformation.tx) / this->transformation.sx) / this->dataPointWidth);
			if (idx <= 0 || idx >= data.size())
				break;
			wchar_t buf[256] = { 0 };
			tm t;
			gmtime_s(&t, &data[idx].time);
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

IAxis* Axis::SetDataType(const AxisDataType& type)
{
	this->axisDataType = type;
	return this;
}
const AxisType& Axis::GetAxisType()
{
	return this->axisType;
}
float Axis::GetAxisSize()
{
	return this->axisSize;
}

std::wstring Axis::GetBufferSource()
{
	return this->bufferName;
}