#pragma once
#include "ProjectOptions.h"
#if FCHART_COMPILE_PLATFORM_WIN_D2D1

#include "Platform.h"
#define NOMINMAX
#include <Windows.h>
#include <windowsx.h>
#undef DrawText
#include <stdint.h>
#include <vector>
#include <map>
#include <d2d1.h>
#include <dwrite.h>



namespace fchart
{
	
	class BrushRef
		: public Object, public IBrush
	{
	public:
		BrushRef(ID2D1Brush *brush, BrushProperties bp);
		virtual ~BrushRef() override;
		virtual void GetProperties(BrushProperties& out) override;
		ID2D1Brush* GetBrush();
	private:
		ID2D1Brush *pBrush;
		BrushProperties props;
	};


	class PlatformD2D1 : 
		public IPlatform,
		public Object
	{
	public:
		PlatformD2D1(HWND hWnd,int x, int y, int width, int height);


		virtual ~PlatformD2D1() override;
		virtual void AddEventMouseMove( IMouseMoveListener* listener) override;
		virtual void RemoveEventMouseMove( IMouseMoveListener* listener) override;

		virtual void SetSize(const int32_t& width, const int32_t& height) override;

		//drawers
		virtual void Begin() override;
		virtual void End() override;
		virtual IBrush* CreateBrush(const BrushProperties& props) override;
		virtual void RemoveBrush(const int32_t& id) override;
		virtual int32_t CreateTextFormat(const TextFormatProperties& props) override;
		virtual void RemoveTextFormat(const int32_t& id) override;
		virtual void SetTextFormat(const int32_t& id) override;
		virtual void SetBrush(IBrush* pBrush, const BrushStyle& style) override;
		virtual void SetBrushFill(const int32_t& id) override;
		virtual void SetBrushOutline(const int32_t& id) override;

		virtual void ClearRect(const Rect& rc) override;

		virtual void DrawText(const Point& p, const wchar_t *str) override;


		virtual void DrawLine(const Point& p0, const Point& p1) override;
		virtual void DrawCandlestick(const float y[4], const float& x) override;

	
	private:
		Point ScreenToClientCoords(const Point& p);
		int32_t GenerateId();

	private:
		HWND hWnd;
		ID2D1Factory *pFactory;
		ID2D1HwndRenderTarget *pRender;
		IDWriteFactory *pWriteFactory;
		IDWriteTextFormat *pTextFormat;
		ID2D1Brush *pBrushFill;
		ID2D1Brush *pBrushOutline;

		D2D1::Matrix3x2F baseMatrix;
		D2D1::Matrix3x2F translateMatrix;
		D2D1::Matrix3x2F scaleMatrix;

		int32_t _lastId;

		std::vector<IMouseMoveListener*> mouseMoveCallbacks;
		std::map<int32_t, ID2D1Brush*> brushes;
		std::map<int32_t, IDWriteTextFormat*> textFormats;
		

		
	private:
		static LRESULT CALLBACK ControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	};

}
#endif