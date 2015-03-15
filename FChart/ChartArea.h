#pragma once
#include "api.h"
#include "Platform.h"
#include "Axis.h"
#include "Series.h"
#include <map>

namespace fchart
{


	class Chart;
	class ChartArea 
		: public IChartArea, public virtual Object, public IMouseMoveListener
	{
	public:
		ChartArea(IPlatform *pPlatform, Chart *pChart);
		virtual ~ChartArea();
		
		virtual IChartArea* SetRect(const Rect& rc);
		virtual void SetBackground(const int32_t& color, bool drawBg = true);
		virtual void Draw();
		virtual void OnMouseMove(const MouseEventArgs& args) override;
		virtual IAxis* CreateAxis(const wchar_t* name,const AxisType& type) override;
		virtual IAxis* GetAxis(const wchar_t* name) override;
		virtual ISeries* CreateSeries(const wchar_t* name) override;
		virtual ISeries* GetSeries(const wchar_t* name) override;
		virtual IChartArea* SetXAxisSync(const bool& onOff) override;
		virtual bool IsXAxisSync() override;

		const Rect& GetRect();
		void SetTransformation(const Transformation& trans);
		const Transformation& GetTransformation();
	


	private:
		IPlatform *pPlatform;
		Rect rcArea;
		Rect rcSeries;
		IBrush *pBrush;
		std::map<std::wstring,Axis*> axies;
		std::map<std::wstring,Series*> series;
		std::vector<Quotation> data;
		IBrush *pBrushRed, *pBrushGreen;
		Chart* pChart;

		/*Test*/
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