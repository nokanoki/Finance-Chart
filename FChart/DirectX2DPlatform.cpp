#include "ProjectOptions.h"
#if FCHART_COMPILE_PLATFORM_WIN_D2D1
#include "DirectX2DPlatform.h"
#include <exception>
#include <algorithm>
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
using namespace fchart;

/*
encapsulation of platform
*/
IPlatform* CreateD2D1Platform(HWND hWnd, int32_t x, int32_t y, int32_t width, int32_t height)
{
	return new PlatformD2D1(hWnd, x, y, width, height);
}


/*
BrushRef imp
*/
BrushRef::BrushRef(ID2D1Brush *brush, BrushProperties bp) :
pBrush(brush), props(bp)
{
	
}
BrushRef::~BrushRef()
{
	this->pBrush->Release();
}
void BrushRef::GetProperties(BrushProperties& out)
{
	out = this->props;
}
ID2D1Brush* BrushRef::GetBrush()
{
	return this->pBrush;
}


__inline D2D1::ColorF makecolorf(const int32_t& color){
	return D2D1::ColorF(
		static_cast<UINT32>(color),
		float((color >> 24) & 0xff) / 255.f
		);
}
__inline D2D1_POINT_2F makepoint2f(const Point& p){
	return D2D1::Point2F(p.x, p.y);
}


PlatformD2D1::PlatformD2D1(HWND hParrent, int x, int y, int width, int height) :
pRender(nullptr), pFactory(nullptr), pBrushOutline(nullptr), pBrushFill(nullptr),
pWriteFactory(nullptr), pTextFormat(nullptr), _lastId(0)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof wc;
	wc.lpszClassName = L"clsPlatformWND";
	wc.lpfnWndProc = PlatformD2D1::ControlProc;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	RegisterClassEx(&wc);

	this->hWnd = CreateWindowEx(0, wc.lpszClassName, NULL, WS_CHILD | WS_VISIBLE, x, y, width, height, hParrent, NULL, NULL, reinterpret_cast<LPVOID>(this));

	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &this->pFactory)))
		throw std::exception(__FUNCTION__ " D2D1Factory initialization failed");
	if (FAILED(this->pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties(hWnd, D2D1::SizeU(width, height),D2D1_PRESENT_OPTIONS_IMMEDIATELY), &this->pRender)))
		throw std::exception(__FUNCTION__ " D2D1Render initialization failed");
	if (FAILED(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>( &this->pWriteFactory))))
		throw std::exception(__FUNCTION__ " DWriteFactory initialization failed");


	this->pRender->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
	this->baseMatrix = D2D1::Matrix3x2F::Rotation(180.f, D2D1::Point2F(width / 2, height / 2))
		* D2D1::Matrix3x2F::Scale(D2D1::SizeF(-1.f, 1.f))
		* D2D1::Matrix3x2F::Translation(D2D1::SizeF(width,0));
	this->pRender->SetTransform(this->baseMatrix);
	this->translateMatrix = D2D1::Matrix3x2F::Identity();
	this->scaleMatrix = D2D1::Matrix3x2F::Identity();
	



}
PlatformD2D1::~PlatformD2D1()
{
	if (this->pFactory) this->pFactory->Release();
	if (this->pRender) this->pRender->Release();
	if (this->pWriteFactory) this->pWriteFactory->Release();
	

	for (auto brush : this->brushes)
		brush.second->Release();
	for (auto textformat : this->textFormats)
		textformat.second->Release();
}

void PlatformD2D1::AddEventMouseMove(IMouseMoveListener* listener)
{
	this->mouseMoveCallbacks.push_back(listener);
}
void PlatformD2D1::RemoveEventMouseMove( IMouseMoveListener* listener)
{
	auto find = std::find(this->mouseMoveCallbacks.begin(), this->mouseMoveCallbacks.end(), listener);
	this->mouseMoveCallbacks.erase(find);
}

void PlatformD2D1::SetSize(const int32_t& width, const int32_t& height)
{
	SetWindowPos(this->hWnd, NULL, 0, 0, width, height, SWP_NOMOVE);
	this->pRender->Resize(D2D1::SizeU(width, height));
	
	this->baseMatrix = D2D1::Matrix3x2F::Rotation(180.f, D2D1::Point2F(width / 2.f, height / 2.f))
		* D2D1::Matrix3x2F::Scale(D2D1::SizeF(-1.f, 1.f))
		* D2D1::Matrix3x2F::Translation(D2D1::SizeF(width, 0));
	this->pRender->SetTransform(this->baseMatrix * this->scaleMatrix * this->translateMatrix);
}



void PlatformD2D1::Begin()
{
	this->pRender->BeginDraw();
	this->pRender->Clear(D2D1::ColorF(D2D1::ColorF::Black));
	this->pRender->SetTransform(this->baseMatrix * this->scaleMatrix * this->translateMatrix);
	
}
void PlatformD2D1::End()
{
	this->pRender->EndDraw();
}
IBrush* PlatformD2D1::CreateBrush(const BrushProperties& props)
{
	
	if (props.type == BPT_SOLID)
	{
		
		ID2D1SolidColorBrush *obj = nullptr;
		/*todo checks*/
		this->pRender->CreateSolidColorBrush(makecolorf(props.color[0]), &obj);
		return new BrushRef(obj, props);
	}
}
void PlatformD2D1::RemoveBrush(const int32_t& id)
{
	this->brushes[id]->Release();
	this->brushes.erase(id);
}
int32_t PlatformD2D1::CreateTextFormat(const TextFormatProperties& props)
{
	/*todo checks*/
	IDWriteTextFormat *obj = nullptr;
	int32_t id = this->GenerateId();
	this->pWriteFactory->CreateTextFormat(
		props.family,
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		props.fontSize,
		L"en-us",
		&obj);
	this->textFormats[id] = obj;
	return id;

}
void PlatformD2D1::RemoveTextFormat(const int32_t& id)
{
	this->textFormats.erase(id);
}
void PlatformD2D1::SetTextFormat(const int32_t& id)
{
	auto find = this->textFormats.find(id);
	if (find == this->textFormats.end())
		throw std::exception(__FUNCTION__ " invalid id");
	this->pTextFormat = find->second;
}
void PlatformD2D1::SetBrush(IBrush *pBrush, const BrushStyle& style)
{
	switch (style)
	{
	case BrushStyle::Fill:
		this->pBrushFill = dynamic_cast<BrushRef*>(pBrush)->GetBrush();
		break;
	case BrushStyle::Outline:
		this->pBrushOutline = dynamic_cast<BrushRef*>(pBrush)->GetBrush();
	default:
		break;
	}
}
void PlatformD2D1::SetBrushFill(const int32_t& id)
{
	auto find = this->brushes.find(id);
	if (find == this->brushes.end())
		throw std::exception(__FUNCTION__ " invalid id");
	this->pBrushFill = find->second;
}
void PlatformD2D1::SetBrushOutline(const int32_t& id)
{
	auto find = this->brushes.find(id);
	if (find == this->brushes.end())
		throw std::exception(__FUNCTION__ " invalid id");
	this->pBrushOutline = find->second;
}

void PlatformD2D1::ClearRect(const Rect& rc)
{
	this->pRender->FillRectangle(D2D1::RectF(rc.left, rc.top, rc.right, rc.bottom), this->pBrushFill);
}

void PlatformD2D1::DrawText(const Point& p, const wchar_t *str)
{
	int32_t sizeX = 0;
	int32_t sizeY = 1;
	int32_t temp = 0;
	int32_t totalSize = 0;
	if (!str || !*str)
		return;
	for (const wchar_t* it = str; *it != 0; it++)
	{
		if (*it == L'\n')
		{
			sizeY++;
			sizeX = std::max(sizeX, temp);
			temp = 0;
		}
		temp++;
		totalSize++;
	}
	sizeX = std::max(sizeX, temp);
	if (!this->pTextFormat)
		throw std::exception(__FUNCTION__ " there no selected TextFormat");
	float fontSize = this->pTextFormat->GetFontSize();
	D2D1_RECT_F rc;
	D2D1_POINT_2F p0 = this->baseMatrix.TransformPoint(makepoint2f(p));
	
	rc.top = p0.y;
	rc.left = p0.x;
	rc.right = p0.x + fontSize * sizeX;
	rc.bottom =  p0.y + fontSize * sizeY;
	
	this->pRender->SetTransform(D2D1::Matrix3x2F::Identity());
	this->pRender->DrawText(str, totalSize, this->pTextFormat, rc, this->pBrushOutline);
	this->pRender->SetTransform(this->baseMatrix * this->scaleMatrix * this->translateMatrix);
}

void PlatformD2D1::DrawLine(const Point& p0, const Point& p1)
{
	this->pRender->DrawLine(makepoint2f(p0), makepoint2f(p1), this->pBrushFill);
	
}
void PlatformD2D1::DrawCandlestick(const float y[4], const float& x)
{
	D2D1_RECT_F body;
	body.top = y[2];
	body.bottom = y[3];
	body.left = x - 4;
	body.right = x + 4;

	D2D1_POINT_2F topString[2], botString[2];

	topString[0].x = x;
	topString[0].y = y[0];
	topString[1].x = x;
	topString[1].y = y[2];

	botString[0].x = x;
	botString[0].y = y[1];
	botString[1].x = x;
	botString[1].y = y[3];
	if (!this->pBrushFill)
		throw std::exception(__FUNCTION__ " invalid fill brush.");
	if (!this->pBrushOutline)
		throw std::exception(__FUNCTION__ " invalid outline brush");

	this->pRender->FillRectangle(body, this->pBrushFill);
	this->pRender->DrawRectangle(body, this->pBrushOutline);
	
	this->pRender->DrawLine(topString[0], topString[1], this->pBrushOutline);
	this->pRender->DrawLine(botString[0], botString[1], this->pBrushOutline);
	

}



/*
other
*/

Point PlatformD2D1::ScreenToClientCoords(const Point& p)
{
	auto out = this->baseMatrix.TransformPoint(makepoint2f(p));
	return makepoint(out.x, out.y);
}
int32_t PlatformD2D1::GenerateId()
{
	return this->_lastId++;
}

/*
Wnd proc
*/
LRESULT CALLBACK PlatformD2D1::ControlProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PlatformD2D1 *_this = reinterpret_cast<PlatformD2D1*>(GetWindowLong(hWnd, GWLP_USERDATA));
	CREATESTRUCT *createStruct = nullptr;
	switch (msg)
	{
	case WM_CREATE:
		createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
		SetWindowLong(hWnd, GWL_USERDATA, reinterpret_cast<LONG>(createStruct->lpCreateParams));
	
		break;
	case WM_MOUSEMOVE:
	{
		MouseEventArgs args;
		Point p = _this->ScreenToClientCoords(makepoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
		args.x = p.x;
		args.y = p.y;
		
		args.buttonState.left = (bool) (wParam & MK_LBUTTON);
		args.buttonState.right = (bool) (wParam & MK_RBUTTON);
		args.buttonState.mid = (bool) (wParam & MK_MBUTTON);

		for (auto listener : _this->mouseMoveCallbacks)
			listener->OnMouseMove(args);
	}
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam); 
	}
	return 0;
}
#endif