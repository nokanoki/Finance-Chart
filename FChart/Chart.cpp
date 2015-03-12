#include "Chart.h"

using namespace fchart;


Chart::Chart(IPlatform *platform)
	:pPlatform(platform)
{
	platform->AddRef();
	platform->AddEventMouseMove(this);
}
Chart::~Chart()
{
	for (auto chartArea : this->chartAreas)
		chartArea->Release();
	pPlatform->Release();
}
void Chart::SetSize(const int32_t& width, const int32_t& height)
{
	this->pPlatform->SetSize(width, height);
	this->Render();
}

IChartArea* Chart::CreateChartArea()
{
	return new ChartArea(pPlatform);
}
void Chart::AddChartArea(IChartArea *pChartArea)
{
	this->chartAreas.push_back(dynamic_cast<ChartArea*> (pChartArea));
	this->pPlatform->AddEventMouseMove(dynamic_cast<ChartArea*>(pChartArea));
	pChartArea->AddRef();
}

void Chart::Render()
{
	this->pPlatform->Begin();
	for (auto chartArea : this->chartAreas)
		chartArea->Draw();
	this->pPlatform->End();
}

void Chart::OnMouseMove(const MouseEventArgs& e)
{
	this->Render();
}