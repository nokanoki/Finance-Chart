#pragma once
#include "api.h"
#include "Platform.h"
#include "Axis.h"
#include "Series.h"

namespace fchart
{
	class ChartArea 
		: public Object,
		public IMouseMoveListener
	{
	public:
		ChartArea(IPlatform *pPlatform);
		virtual ~ChartArea();
		
		virtual void SetRect(const Rect& rc);
		virtual void SetBackground(const int32_t& color, bool drawBg = true);

		virtual void Draw();

		virtual void OnMouseMove(const MouseEventArgs& args) override;

	private:
		IPlatform *pPlatform;
		Rect rcArea;
		IBrush *pBrush;
		Axis *pAxis;
		Series *pSeries;
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