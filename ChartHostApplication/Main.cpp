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
	s.hWnd = hWnd;
	s.x = 0;
	s.y = 0;
	s.height = rc.bottom - rc.top;
	s.width = rc.right - rc.left;
	s.ppOut = reinterpret_cast<void**>(&chart);
	factory(&s);
	initchart();
	

	ShowWindow(hWnd, nShow);
	UpdateWindow(hWnd);
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
	
	chart
		->SetAreaChartPositionType(fchart::ChartAreaPositionType::Stack)
		->GetChartArea(L"default")
		//->SetRect(fchart::makerect(100.f,800.f,800.f,400.f))
		->CreateSeries(L"price")
		->AddData(q, _countof(q))
		->SetSeriesType(fchart::SeriesType::Candlestick);
	chart
		->GetChartArea(L"default")
		->GetAxis(L"default x")
		->SetDataType(fchart::AxisDataType::Date)
		->SetSourceSeries(L"price");
	
	chart
		->CreateChartArea(L"chartArea")
		//->SetRect(fchart::makerect(100.f,300.f,800.f,50.f))
		->CreateSeries(L"price")
		->AddData(q, _countof(q))
		->SetSeriesType(fchart::SeriesType::Band);

	chart
		->GetChartArea(L"chartArea")
		->SetXAxisSync(true)
		->GetAxis(L"default x")
		->SetDataType(fchart::AxisDataType::Date)
		->SetSourceSeries(L"price");

	chart
		->CreateChartArea(L"chartArea1")
		->CreateSeries(L"price")
		->AddData(q, _countof(q));

	chart
		->GetChartArea(L"chartArea1")
		->SetXAxisSync(true)
		->GetAxis(L"default x")
		->SetSourceSeries(L"price")
		->SetDataType(fchart::AxisDataType::Date);
	
	
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