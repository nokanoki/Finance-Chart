#pragma once
#include "api.h"
#include "Object.h"
#include "Platform.h"
#include "ChartArea.h"
#include <map>
namespace fchart
{
	class Chart :
		public IChart,
		public IMouseMoveListener,
		public Object
	{
	public:
		Chart(IPlatform *pPlatform, const int32_t& width, const int32_t& height);
		virtual ~Chart() override;

		virtual void SetSize(const int32_t& width, const int32_t& height) override;

		virtual IChartArea* CreateChartArea(const wchar_t* name) override;
		virtual IChartArea* GetChartArea(const wchar_t* name) override;
		virtual void Render();

		virtual void OnMouseMove(const MouseEventArgs& e) override;

	private:
		Rect rcChart;
		IPlatform *pPlatform;
		std::map<std::wstring,ChartArea*> chartAreas;

	};
}