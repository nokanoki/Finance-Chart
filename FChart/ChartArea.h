#pragma once
#include "api.h"
#include "Platform.h"
#include "Axis.h"

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
	
	};
}