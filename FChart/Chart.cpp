#include "Chart.h"

using namespace fchart;


Chart::Chart(IPlatform *platform, const int32_t& width, const int32_t& height)
	:pPlatform(platform)
{
	this->rcChart = makerect(0.f, static_cast<float>(height), static_cast<float>(width), 0.f);
	platform->AddRef();
	platform->AddEventMouseMove(this);
	this->CreateChartArea(L"default");

}
Chart::~Chart()
{
	for (auto chartArea : this->chartAreas)
		chartArea.second->Release();
	pPlatform->Release();
}
void Chart::SetSize(const int32_t& width, const int32_t& height)
{
	this->pPlatform->SetSize(width, height);
	this->rcChart = makerect(0.f, static_cast<float>(height), static_cast<float>(width), 0.f);
	this->Render();
}

IChartArea* Chart::CreateChartArea(const wchar_t* name)
{
	auto area = new ChartArea(pPlatform);
	this->pPlatform->AddEventMouseMove(area);
	area->SetRect(this->rcChart);
	this->chartAreas[name] = area;
	return area;
}
IChartArea* Chart::GetChartArea(const wchar_t* name)
{
	return this->chartAreas[name];
}

void Chart::Render()
{
	this->pPlatform->Begin();
	for (auto chartArea : this->chartAreas)
		chartArea.second->Draw();
	this->pPlatform->End();
}

void Chart::OnMouseMove(const MouseEventArgs& e)
{
	this->Render();
}