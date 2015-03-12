#include "Series.h"
using namespace fchart;


Series::Series(IPlatform *pPlatform) :
pPlatform(pPlatform)
{
	pPlatform->AddRef();
	this->pBrushGreen = this->pPlatform->CreateBrush(makesolidbrushprps(0xff00ff00));
	this->pBrushRed = this->pPlatform->CreateBrush(makesolidbrushprps(0xffFF0000));
	this->pBrushWhite = this->pPlatform->CreateBrush(makesolidbrushprps(0xffFFffFF));

	this->transformation.sx = 1.f;
	this->transformation.sy = 1.f;
	this->transformation.tx = 0.f;
	this->transformation.ty = 0.f;
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
void Series::AddData(const Quotation *pData, const int32_t& count)
{
	std::copy(pData, pData + count, std::back_inserter(this->data));
}
void Series::Draw()
{
	int i = 0;
	this->pPlatform->SetBrush(this->pBrushGreen, BrushStyle::Fill);
	this->pPlatform->SetBrush(this->pBrushWhite, BrushStyle::Outline);
	for (auto q : this->data)
	{
		float a = transformation.sy ;
		float y[] = { 
			q.high * a + transformation.ty, 
			q.low * a + transformation.ty,
			q.open * a + transformation.ty,
			q.close * a + transformation.ty };
		float x = (i += 10) * transformation.sx + transformation.tx;

		//is up
		if (y[2] <= y[3])
		{
			this->pPlatform->SetBrush(this->pBrushGreen, BrushStyle::Fill);
			std::swap(y[2], y[3]);
		}
		else
		{
			this->pPlatform->SetBrush(this->pBrushRed, BrushStyle::Fill);
			;
		}
		
		this->pPlatform->DrawCandlestick(y , x);
	}
}
