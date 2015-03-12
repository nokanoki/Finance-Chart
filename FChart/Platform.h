#pragma once
#include "Object.h"
#include <string>
#include <functional>
#include <stdint.h>
namespace fchart
{
	

	enum class BrushType { Solid, LinearGradient };
	struct BrushProperties
	{
		BrushType type;
		int32_t colorCount;
		int32_t color[1];
	};
	__inline int32_t makergba(const int32_t& r, const int32_t& g, const int32_t& b, const int32_t& a = 0xff){
		return (a << 24) | (r << 16) | (g << 8) | (b);
	}
	__inline BrushProperties makesolidbrushprps(const int32_t& color){
		BrushProperties bp;
		bp.colorCount = 1;
		bp.type = BrushType::Solid;
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
		tfp.fontSize = 12.f;
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
	class ITextFormat :
		public virtual IObject
	{
	public:
		virtual ~ITextFormat() = default;
		virtual void GetProperties(TextFormatProperties& out) = 0;
	};

	enum class BrushStyle{ Fill, Outline };
	enum class StrokeStyle { Solid, Dot, Dash };

	class IPlatform
		: public virtual IObject
	{


	public:
		virtual ~IPlatform() = default;
		virtual void AddEventMouseMove(IMouseMoveListener* listener) = 0;
		virtual void RemoveEventMouseMove(IMouseMoveListener* listener) = 0;
		virtual void SetSize(const int32_t& width, const int32_t& height) = 0;
		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual IBrush* CreateBrush(const BrushProperties& props) = 0;
		virtual ITextFormat* CreateTextFormat(const TextFormatProperties& props) = 0;
		virtual void SetTextFormat(ITextFormat* pFormat) = 0;
		virtual void SetBrush(IBrush* pBrush, const BrushStyle& style) = 0;
		

		virtual void DrawRect(const Rect& rc, const BrushStyle& style) = 0;
		virtual void DrawText(const Point& p, const wchar_t *str) = 0;
		virtual void DrawLine(const Point& p0, const Point& p1, const StrokeStyle& strokeStyle = StrokeStyle::Solid) = 0;

		/*
		y => high low open close
		*/
		virtual void DrawCandlestick(const float y[4], const float& x) = 0;

	};
}