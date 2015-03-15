#pragma once
#include "api.h"
#include "Platform.h"
#include <vector>


namespace fchart
{
	class ChartArea;
	enum class SeriesFocus { Auto, Manual };
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
		//virtual ISeries* AddData(const Quotation* pData, const int32_t& count) override;
		//virtual void Draw();


		virtual ISeries* SetBufferSource(const wchar_t* bufferName);
		void Draw(const std::vector<Quotation>& buffer);
		std::wstring GetBufferSourceName();

	

		//test
		//std::vector<Quotation> GetData();

	private:
		IPlatform *pPlatform;
		Rect rcSeries;
		IBrush *pBrushRed;
		IBrush *pBrushGreen;
		IBrush *pBrushWhite;
		ChartArea *pChartArea;
		float dataPointWidth;
		SeriesType seriesType;
		SeriesFocus seriesFocus;
		Transformation transformation;

		//test
		std::wstring bufferName;
	};
}