#pragma once
#include "ProjectOptions.h"
#include <stdint.h>
namespace fchart
{
	struct Point
	{
		float x, y;
	};
	__inline Point makepoint(const float& x, const float& y){
		Point p;
		p.x = x;
		p.y = y;
		return p;
	}
	struct Rect
	{
		float left;
		float top;
		float right;
		float bottom;
	};

	__inline Rect makerect(const float& left, const float& top, const float& right, const float& bottom){
		Rect rc;
		rc.top = top;
		rc.left = left;
		rc.right = right;
		rc.bottom = bottom;
		return rc;
	}
	struct Quotation
	{
		time_t time;
		float high, low, open, close;
		int volume;
	};
	struct Transformation
	{
		float sx, sy;
		float tx, ty;
	};
	__inline Transformation maketransformation(const float& sx = 1.f, const float& sy = 1.f, const float& tx = 0.f, const float& ty = 0.f){
		Transformation t;
		t.sx = sx;
		t.sy = sy;
		t.tx = tx;
		t.ty = ty;
		return t;
	}
	class IObject
	{
	public:
		virtual ~IObject() = default;
		virtual void Release() = 0;
		virtual void AddRef() = 0;
	};	
	enum class AxisType{ Horizontal, Vertical };
	enum class AxisPosition { Left, Top, Right, Bottom, UserDefine };
	enum class AxisDataType { Price, Time, Date };
	
	class IAxis
	{
	public:
		virtual ~IAxis() = default;
		virtual IAxis* SetDataType(const AxisDataType& type) = 0;
		virtual IAxis* SetSourceSeries(const wchar_t* name) = 0;
	};
	enum class SeriesType{ Candlestick, Line, Band, Bar };
	class ISeries
	{
	public:
		~ISeries() = default;
		virtual ISeries* AddData(const Quotation*, const int32_t& count) = 0;
		virtual ISeries* SetSeriesType(const SeriesType& type) = 0;
		
	};

	class IChartArea
	{
	public:
		virtual ~IChartArea() = default;
		virtual IAxis* CreateAxis(const wchar_t* name, const AxisType& type) = 0;
		virtual IAxis* GetAxis(const wchar_t* name) = 0;
		virtual IChartArea* SetRect(const Rect& rc) = 0;
		virtual ISeries* CreateSeries(const wchar_t* name) = 0;
		virtual ISeries* GetSeries(const wchar_t* name) = 0;
		virtual IChartArea* SetXAxisSync(const bool& onOff) = 0;
		virtual bool IsXAxisSync() = 0;

	};
	enum class ChartAreaPositionType{ Stack };

	class IChart
		: public virtual IObject
	{
	public:
		virtual ~IChart() = default;
		virtual void SetSize(const int32_t& width, const int32_t& height) = 0;
		virtual IChartArea* CreateChartArea(const wchar_t* name) = 0;
		virtual IChartArea* GetChartArea(const wchar_t* name) = 0;
		virtual IChart* SetAreaChartPositionType(const ChartAreaPositionType& type) = 0;
		virtual void Render() = 0;

	};
}















typedef struct FCHART_FACTORY_STRUCT_tag
{
	void **ppOut;
#if FCHART_COMPILE_PLATFORM_WIN_D2D1
	void* hWnd;
	int32_t x;
	int32_t y;
	int32_t width;
	int32_t height;
#endif
}FCHART_FACTORY_STRUCT;
extern "C"
{
	EXPORT int32_t fchart_factory(FCHART_FACTORY_STRUCT *pStruct);
}