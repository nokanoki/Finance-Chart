#pragma once
#include "api.h"
#include "Platform.h"
#include <vector>


namespace fchart
{
	class Series
	{
	public:
		Series(IPlatform *pPlatform);
		virtual ~Series();
		virtual void SetRect(const Rect& rc);
		virtual void SetTransformation(const Transformation& trans);
		virtual void AddData(const Quotation* pData, const int32_t& count);
		virtual void Draw();

	private:
		IPlatform *pPlatform;
		Rect rcSeries;
		IBrush *pBrushRed;
		IBrush *pBrushGreen;
		IBrush *pBrushWhite;

		std::vector<Quotation> data;
		Transformation transformation;
	};
}