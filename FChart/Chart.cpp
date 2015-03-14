#include "Chart.h"

using namespace fchart;


Chart::Chart(IPlatform *platform, const int32_t& width, const int32_t& height)
	:pPlatform(platform)
{
	this->rcChart = makerect(0.f, static_cast<float>(height), static_cast<float>(width), 0.f);
	platform->AddRef();
	platform->AddEventMouseMove(this);
	this->CreateChartArea(L"default");
	this->transformation = maketransformation();

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
			
			auto first = this->chartAreas.front();
			Rect rc = first.second->GetRect();
			rc.top = this->rcChart.top;
			rc.left = this->rcChart.left;
			rc.right = this->rcChart.right;
			first.second->SetRect(rc);
			for (auto a : this->chartAreas)
			{
				rc = a.second->GetRect();
				rc.left = this->rcChart.left;
				rc.right = this->rcChart.right;
				a.second->SetRect(rc);
			}
		}
	}

	this->Render();
}

IChartArea* Chart::CreateChartArea(const wchar_t* name)
{
	auto area = new ChartArea(pPlatform, this);
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

	/*
	if (e.x < this->rcChart.left
		|| e.x > this->rcChart.right
		|| e.y > this->rcChart.top
		|| e.y < this->rcChart.bottom)
		return;
	if (e.buttonState.left)
	{
		if (!mouse.isDragging)
		{
			mouse.isDragging = true;
			mouse.x = mouse.xstart = mouse.xlast = e.x;
			mouse.y = mouse.ystart = mouse.ylast = e.y;
		}
		else
		{
			mouse.xlast = mouse.x;
			mouse.ylast = mouse.y;
			mouse.x = e.x;
			mouse.y = e.y;
		}
	}
	else
		mouse.isDragging = false;

	if (mouse.isDragging)
	{
		transformation.tx += mouse.x - mouse.xlast;
		transformation.ty += mouse.y - mouse.ylast;

		for (auto a : this->chartAreas)
		{
			auto t = a.second->GetTransformation();
			t.tx += mouse.x - mouse.xlast;
			a.second->SetTransformation(t);
		}
	}
	*/
	this->Render();
}

std::list<std::pair<std::wstring, ChartArea*>> Chart::GetChartAreas()
{
	return this->chartAreas;
}
IChart* Chart::SetAreaChartPositionType(const ChartAreaPositionType& type)
{
	this->chartAreaPositionType = type;
	return this;
}