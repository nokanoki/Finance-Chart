#pragma once
#include "api.h"
#include "Platform.h"
#include "Axis.h"
#include "Series.h"
#include <map>

namespace fchart
{
	

	class Chart;
	struct Buffer;
	class ChartArea
		: public IChartArea, public IMouseMoveListener
	{
	public:
		ChartArea() = delete;
		ChartArea(const ChartArea&) = delete;

		ChartArea(IPlatform *pPlatform, Chart *pChart);
		~ChartArea();

		void SetBackground(const int32_t& color, bool drawBg = true);
		void Draw(const std::map <std::wstring, Buffer>& buffers);
		const Rect& GetRect();
		void SetTransformation(const Transformation& trans);
		const Transformation& GetTransformation();

	private:
		void DrawAxisVertical();


	public://///////////// event imp
		virtual void OnMouseMove(const MouseEventArgs& args) override;

	public://////////////// interface imp
	
		virtual IChartArea* SetRect(const Rect& rc);
		virtual ISeries* CreateSeries(const wchar_t* name) override;
		virtual ISeries* GetSeries(const wchar_t* name) override;
		virtual IChartArea* SetXAxisSync(const bool& onOff) override;
		virtual IChartArea* FocusLast(const wchar_t* seriesName) override;
		virtual IChartArea* SetYBoundsTest(const float& max, const float& min) override;
		virtual bool IsXAxisSync() override;

	private:
		IPlatform *pPlatform;
		Rect rcArea;
		Rect rcSeries;
		Rect rcAxisY;
		IBrush *pBrush;
		
		ITextFormat *pTextFormat;
		IBrush *pAxisYTextBrush;
		std::map<std::wstring, Series*> series;
		Chart* pChart;
		bool isXAxisSync;
		Transformation transformation;
		struct
		{
			bool isDragging;
			float xstart, ystart;
			float x, y;
			float xlast, ylast;
		}mouse;
		
	};
}