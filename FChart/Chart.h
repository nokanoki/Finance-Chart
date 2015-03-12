#pragma once
#include "api.h"
#include "Object.h"
#include "Platform.h"
#include "ChartArea.h"
namespace fchart
{
	class Chart :
		public IChart,
		public IMouseMoveListener,
		public Object
	{
	public:
		Chart(IPlatform *pPlatform);
		virtual ~Chart() override;

		virtual void SetSize(const int32_t& width, const int32_t& height) override;

		virtual IChartArea* CreateChartArea() override;
		virtual void AddChartArea(IChartArea* chartArea) override;

		virtual void Render();

		virtual void OnMouseMove(const MouseEventArgs& e) override;

	private:
		IPlatform *pPlatform;
		std::vector<ChartArea*> chartAreas;

	};
}