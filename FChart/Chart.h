#pragma once
#include "api.h"
#include "Object.h"
#include "Platform.h"
#include "ChartArea.h"
#include <map>
#include <list>
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
		virtual IChart* SetAreaChartPositionType(const ChartAreaPositionType& type) override;
		virtual void Render();

		virtual void OnMouseMove(const MouseEventArgs& e) override;


		std::list<std::pair<std::wstring, ChartArea*>> GetChartAreas();

	private:
		Rect rcChart;
		IPlatform *pPlatform;
		ChartAreaPositionType chartAreaPositionType;
		std::list<std::pair<std::wstring,ChartArea*>> chartAreas;

		//test
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