#pragma once
#include "api.h"
#include "Platform.h"
#include <vector>


namespace fchart
{
	class ChartArea;
	class Series
		: public ISeries, public virtual Object
	{
	public:
		Series(IPlatform *pPlatform,ChartArea *charArea);
		virtual ~Series();
		virtual void SetRect(const Rect& rc);
		const Rect& GetRect();
		virtual void SetTransformation(const Transformation& trans);
		virtual ISeries* SetSeriesType(const SeriesType& type) override;
	

		virtual ISeries* SetBufferSource(const wchar_t* bufferName);
		void Draw(const std::vector<Quotation>& buffer);
		std::wstring GetBufferSourceName();


	private:
		IPlatform *pPlatform;
		Rect rcSeries;
		IBrush *pBrushRed;
		IBrush *pBrushGreen;
		IBrush *pBrushWhite;
		ChartArea *pChartArea;
		float dataPointWidth;
		SeriesType seriesType;
		Transformation transformation;
		std::wstring bufferName;
	};
}