#pragma once
#include "api.h"
#include "Platform.h"
#include "Axis.h"
#include "Series.h"

namespace fchart
{
	class ChartArea 
		: public IChartArea, public virtual Object, public IMouseMoveListener
	{
	public:
		ChartArea(IPlatform *pPlatform);
		virtual ~ChartArea();
		
		virtual void SetRect(const Rect& rc);
		virtual void SetBackground(const int32_t& color, bool drawBg = true);
		virtual void Draw();
		virtual void OnMouseMove(const MouseEventArgs& args) override;
		virtual IAxis* CreateAxis(const AxisType& type) override;
		virtual void AddAxis(IAxis* pAxis) override;
		virtual ISeries* CreateSeries() override;
		//rename that
		virtual void SetSeries(ISeries* series) override;
		virtual void AddData(const Quotation*, const int32_t& count) override;

	private:
		IPlatform *pPlatform;
		Rect rcArea;
		IBrush *pBrush;
		std::vector<Axis*> axies;
		std::vector<Series*> series;
		std::vector<Quotation> data;
		IBrush *pBrushRed, *pBrushGreen;

		/*Test*/
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