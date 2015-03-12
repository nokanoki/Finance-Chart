#pragma once
#include "api.h"
#include "Platform.h"
#include <vector>


namespace fchart
{
	enum class SeriesType{ Candlestick, Line, Band };

	class Series
		: public ISeries, public virtual Object
	{
	public:
		Series(IPlatform *pPlatform);
		virtual ~Series();
		virtual void SetRect(const Rect& rc);
		virtual void SetTransformation(const Transformation& trans);
		virtual void SetSeriesType(const SeriesType& type); 
		virtual void AddData(const Quotation* pData, const int32_t& count);
		virtual void Draw();

		//test
		std::vector<Quotation> GetData();

	private:
		IPlatform *pPlatform;
		Rect rcSeries;
		IBrush *pBrushRed;
		IBrush *pBrushGreen;
		IBrush *pBrushWhite;
		
		SeriesType seriesType;

		std::vector<Quotation> data;
		Transformation transformation;
	};
}