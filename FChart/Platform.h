#pragma once
#include "Object.h"
#include <string>
#include <functional>
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
	enum BrushPropertiesType { BPT_SOLID, BPT_LINEAR_GRADIENT };
	struct BrushProperties
	{
		int32_t type;
		int32_t colorCount;
		int32_t color[1];
	};
	__inline int32_t makergba(const int32_t& r, const int32_t& g, const int32_t& b, const int32_t& a = 0xff){
		return (a << 24) | (r << 16) | (g << 8) | (b);
	}
	__inline BrushProperties makesolidbrushprps(const int32_t& color){
		BrushProperties bp;
		bp.colorCount = 1;
		bp.type = BPT_SOLID;
		bp.color[0] = color;
		return bp;
	}
	struct TextFormatProperties
	{
		wchar_t family[124];
		float fontSize;
	};
	__inline TextFormatProperties maketextprps()
	{
		TextFormatProperties tfp;
		wcscpy_s(tfp.family, L"Calibria");
		tfp.fontSize = 16.f;
		return tfp;
	}
	struct MouseButtonsState
	{
		unsigned int left : 1;
		unsigned int right : 1;
		unsigned int mid : 1;
	};
	
	struct MouseEventArgs
	{
		float x, y;
		MouseButtonsState buttonState;
	};
	
	class IMouseMoveListener
	{
	public: virtual void OnMouseMove(const MouseEventArgs& args)  = 0;
	};

	class IBrush
		: public virtual IObject
	{
	public:
		virtual ~IBrush() = default;
		virtual void GetProperties(BrushProperties& out) = 0;
	};
	enum class BrushStyle{ Fill, Outline };
	class IPlatform
		: public virtual IObject
	{


	public:
		virtual ~IPlatform() = default;
		virtual void AddEventMouseMove( IMouseMoveListener* listener) = 0;
		virtual void RemoveEventMouseMove( IMouseMoveListener* listener) = 0;

		virtual void SetSize(const int32_t& width, const int32_t& height) = 0;

		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual IBrush* CreateBrush(const BrushProperties& props) = 0;
		virtual void RemoveBrush(const int32_t& id) = 0;
		virtual int32_t CreateTextFormat(const TextFormatProperties& props) = 0;
		virtual void RemoveTextFormat(const int32_t& id) = 0;
		virtual void SetTextFormat(const int32_t& id) = 0;
		virtual void SetBrush(IBrush* pBrush, const BrushStyle& style) = 0;
		virtual void SetBrushFill(const int32_t& id) = 0;
		virtual void SetBrushOutline(const int32_t& id) = 0;

		virtual void ClearRect(const Rect& rc) = 0;

		virtual void DrawText(const Point& p, const wchar_t *str) = 0;


		virtual void DrawLine(const Point& p0, const Point& p1) = 0;

		/*
		y => high low open close
		*/
		virtual void DrawCandlestick(const float y[4], const float& x) = 0;

	};
}