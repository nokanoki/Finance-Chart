#pragma once
#include "ProjectOptions.h"
#include <stdint.h>
namespace fchart
{
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
	class IChart
		: public virtual IObject
	{
	public:
		virtual ~IChart() = default;
		virtual void SetSize(const int32_t& width, const int32_t& height) = 0;
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