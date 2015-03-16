#pragma once
#include "api.h"
#include "Object.h"
#include "Platform.h"
#include "ChartArea.h"
#include <map>
#include <list>
namespace fchart
{
	struct Buffer
	{
		std::vector<Quotation> data;
		IDataManipulator *manipulator;
		std::wstring inputBufferName;
	};


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

		virtual IChart* CreateDataBuffer(const wchar_t* name) override;
		virtual IChart* SetData(const wchar_t* bufferName, const Quotation* pData, const int32_t& count, const SetDataType& type) override;

		const std::vector<Quotation>& GetData(const std::wstring& name);

		virtual IChart* SetDataManipulator(IDataManipulator* obj, const wchar_t* inputBufferName, const wchar_t* outputBufferName) override;

		virtual IChart* UpdateBuffer(const wchar_t* bufferName) override;
	private:
		Rect rcChart;
		IPlatform *pPlatform;
		ChartAreaPositionType chartAreaPositionType;
		std::list<std::pair<std::wstring,ChartArea*>> chartAreas;
		std::map <std::wstring, Buffer> data;

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