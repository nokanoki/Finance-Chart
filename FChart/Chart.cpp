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
	if (this->chartAreaPositionType == ChartAreaPositionType::Stack)
	{
		if (!this->chartAreas.empty())
		{
			
			float height = this->chartAreas.front().second->GetRect().top 
				- this->chartAreas.back().second->GetRect().bottom;
			float space = (this->rcChart.top - this->rcChart.bottom) - height;
			float spacePerArea = space / static_cast<float>(this->chartAreas.size());

			auto rit = this->chartAreas.rbegin();
			Rect rc = rit->second->GetRect();
			rc.top += spacePerArea;
			rit->second->SetRect(rc);
			for (rit++; rit != this->chartAreas.rend(); rit++)
			{
				rc = rit->second->GetRect();
				rc.bottom += spacePerArea;
				rc.top += spacePerArea;
				rit->second->SetRect(rc);
			}
		}
	}

	this->Render();
}

IChartArea* Chart::CreateChartArea(const wchar_t* name)
{
	auto area = new ChartArea(pPlatform);
	this->pPlatform->AddEventMouseMove(area);
	area->SetRect(this->rcChart);
	if (this->chartAreaPositionType == ChartAreaPositionType::Stack)
	{
		if (this->chartAreas.empty())
			area->SetRect(this->rcChart); 
		else
		{
			auto last = this->chartAreas.back().second;
			Rect rc = last->GetRect(); 
			//make space for new area
			rc.bottom += (rc.top - rc.bottom) / 2.f;
			last->SetRect(rc);
			
			//fill the space with the new area
			rc.top = rc.bottom;
			rc.bottom = this->rcChart.bottom;
			area->SetRect(rc);

		}
	}
	this->chartAreas.push_back(std::make_pair(name, area));
	return area;
}
IChartArea* Chart::GetChartArea(const wchar_t* name)
{
	for (auto a : this->chartAreas)
		if (a.first == name)
			return a.second;
	return nullptr;
}

void Chart::Render()
{
	if (this->chartAreaPositionType == ChartAreaPositionType::Stack)
	{
		//just set left right
		for (auto chartArea : this->chartAreas)
		{
			Rect rc = chartArea.second->GetRect();
			rc.left = this->rcChart.left;
			rc.right = this->rcChart.right;
		}
	}

	this->pPlatform->Begin();
	for (auto chartArea : this->chartAreas)
		chartArea.second->Draw();
	this->pPlatform->End();
}

void Chart::OnMouseMove(const MouseEventArgs& e)
{
	this->Render();
}
IChart* Chart::SetAreaChartPositionType(const ChartAreaPositionType& type)
{
	this->chartAreaPositionType = type;
	return this;
}