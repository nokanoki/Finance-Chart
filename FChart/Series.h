#pragma once
#include "api.h"
#include "Platform.h"
#include <vector>


namespace fchart
{
	class ChartArea;
	class Series
		: public ISeries
	{
	public:
		Series() = delete;
		Series(const Series&) = delete;

		Series(IPlatform *pPlatform,ChartArea *charArea);
		~Series();

		void SetRect(const Rect& rc);
		const Rect& GetRect();
		void SetTransformation(const Transformation& trans);
		void Draw(const std::vector<Quotation>& buffer);
		std::wstring GetBufferSourceName();

		////////////// interface imp
	public:
		virtual ISeries* SetSeriesType(const SeriesType& type) override;
		virtual ISeries* SetBufferSource(const wchar_t* bufferName);

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