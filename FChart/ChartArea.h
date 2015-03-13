#pragma once
#include "api.h"
#include "Platform.h"
#include "Axis.h"
#include "Series.h"
#include <map>

namespace fchart
{


	class ChartArea 
		: public IChartArea, public virtual Object, public IMouseMoveListener
	{
	public:
		ChartArea(IPlatform *pPlatform);
		virtual ~ChartArea();
		
		virtual IChartArea* SetRect(const Rect& rc);
		virtual void SetBackground(const int32_t& color, bool drawBg = true);
		virtual void Draw();
		virtual void OnMouseMove(const MouseEventArgs& args) override;
		virtual IAxis* CreateAxis(const wchar_t* name,const AxisType& type) override;
		virtual IAxis* GetAxis(const wchar_t* name) override;
		virtual ISeries* CreateSeries(const wchar_t* name) override;
		virtual ISeries* GetSeries(const wchar_t* name) override;

		const Rect& GetRect();


	private:
		IPlatform *pPlatform;
		Rect rcArea;
		IBrush *pBrush;
		std::map<std::wstring,Axis*> axies;
		std::map<std::wstring,Series*> series;
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