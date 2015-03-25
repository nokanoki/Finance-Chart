#include <Windows.h>
#include "..\\FChart\\api.h"
#include <sstream>
#include <string>
#include <windowsx.h>
#define TEST(exp) try{exp}catch(std::exception e){MessageBoxA(0,e.what(),0,0);}


LRESULT CALLBACK proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int32_t(*factory)(FCHART_FACTORY_STRUCT*) = 0;

void initchart();
fchart::IChart *chart;
fchart::DataManipulator::IFactory *dataFactory;

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShow)
{

	auto lib = LoadLibrary(LR"(..\Debug\FChart.dll)");
	factory = (decltype(factory))GetProcAddress(lib, "fchart_factory");

	WNDCLASSEX wc = { 0 };
	HWND hWnd = NULL;
	MSG msg = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wc.lpszClassName = L"hostApp";
	wc.lpfnWndProc = proc;
	
	RegisterClassEx(&wc);
	hWnd = CreateWindowEx(
		0,
		wc.lpszClassName,
		L"Host App",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL, NULL, hInstance, NULL);
	RECT rc;
	GetWindowRect(hWnd, &rc);
	FCHART_FACTORY_STRUCT s;
	s.id = FCHART_FACTORY_ID_CHART;
	s.hWnd = hWnd;
	s.x = 0;
	s.y = 0;
	s.height = rc.bottom - rc.top;
	s.width = rc.right - rc.left;
	s.ppOut = reinterpret_cast<void**>(&chart);
	factory(&s);
	
	s.id = FCHART_FACTORY_ID_DATAMANIMUPATOR_FACTORY;
	s.ppOut = reinterpret_cast<void**>(&dataFactory);
	factory(&s);

	ShowWindow(hWnd, nShow);
	UpdateWindow(hWnd);
	
	initchart();
	while (GetMessage(&msg,NULL,0,0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	FreeLibrary(lib);
	return 0;
}


fchart::Quotation q[] = 
{
#include "testquotes.array"
};

void initchart()
{
	//TEST TEST TEST TEST TEST 

	auto sma = dataFactory->CreateSMA(20);
	auto rsi = dataFactory->CreateRSI();

	chart
		->CreateDataBuffer(L"data buffer")
		->CreateDataBuffer(L"rsi buffer")
		->CreateDataBuffer(L"sma buffer")
		->SetDataManipulator(sma, L"data buffer", L"sma buffer")
		->SetDataManipulator(rsi, L"data buffer", L"rsi buffer")
		->SetAreaChartPositionType(fchart::ChartAreaPositionType::Stack)
		;
	
	auto topArea = chart->CreateChartArea(L"top area");

	topArea
		->CreateSeries(L"price series")
		->SetBufferSource(L"data buffer")
		->SetSeriesType(fchart::SeriesType::Candlestick)
		;
	topArea
		->CreateSeries(L"sma series")
		->SetBufferSource(L"sma buffer")
		->SetSeriesType(fchart::SeriesType::Line)
		;
	topArea
		->CreateAxis(L"x", fchart::AxisType::Horizontal)
		->SetBufferSource(L"data buffer")
		->SetDataType(fchart::AxisDataType::Date)
		;
	topArea
		->CreateAxis(L"y", fchart::AxisType::Vertical)
		->SetBufferSource(L"data buffer")
		->SetDataType(fchart::AxisDataType::Price)
		;
#if 0

	auto botArea = chart->CreateChartArea(L"bot area");
	botArea
		->CreateSeries(L"rsi series")
		->SetBufferSource(L"rsi buffer")
		->SetSeriesType(fchart::SeriesType::Line)
		;
	botArea
		->CreateAxis(L"x", fchart::AxisType::Horizontal)
		->SetBufferSource(L"rsi buffer")
		->SetDataType(fchart::AxisDataType::Date)
		;
	botArea
		->CreateAxis(L"y", fchart::AxisType::Vertical)
		->SetBufferSource(L"rsi buffer")
		->SetDataType(fchart::AxisDataType::Price)
		;

#endif // 0

	//feed data
	chart->SetData(L"data buffer", q, _countof(q), fchart::SetDataType::Append)
		->UpdateBuffer(L"sma buffer")
		->UpdateBuffer(L"rsi buffer")
		;
	topArea
		->SetYBoundsTest(13.f, 5.f);
	//topArea->FocusLast(L"price series");
	//botArea->FocusLast(L"rsi series");
	//botArea->SetYBoundsTest(100.f, 0.f);
}

LRESULT CALLBACK proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	
	POINT p;
	switch (msg)
	{
	case WM_CREATE:
	
		break;
	case WM_SIZE:
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		//ScreenToClient(hWnd, &p);
		chart->SetSize(p.x,p.y);
		break;
		
	case WM_PAINT:
		
		chart->Render();
		
		//ValidateRect(hWnd, NULL);
		break;
	case WM_CLOSE:
		chart->Release();
		PostQuitMessage(0);
		
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}