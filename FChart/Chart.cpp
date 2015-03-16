#include "Chart.h"

using namespace fchart;
#include "DataManipulator.SMA.h"

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
		delete chartArea.second;
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
		chartArea.second->Draw(this->data);
	this->pPlatform->End();
}

void Chart::OnMouseMove(const MouseEventArgs& e)
{

	
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





IChart* Chart::CreateDataBuffer(const wchar_t* name)
{
	this->data[name].manipulator = nullptr;
	return this;
}
IChart* Chart::SetData(const wchar_t* bufferName, const Quotation* pData, const int32_t& count, const SetDataType& type)
{
	if (type == SetDataType::Append)
		std::copy(pData, pData + count, std::back_inserter(this->data[bufferName].data));

	this->UpdateBuffer(bufferName);

	return this;
}

IChart* Chart::UpdateBuffer(const wchar_t* bufferName)
{
	auto output = this->data.find(bufferName);
	if (output->second.manipulator == nullptr)
		return this;
	if (output == this->data.end())
		return this;
	auto input = this->data.find(output->second.inputBufferName);
	if (input == this->data.end())
		return this;

	output->second.data.resize(input->second.data.size());

	output->second.manipulator->Calc(
		&input->second.data[0],
		&output->second.data[0],
		input->second.data.size()
		);

	return this->UpdateBuffer(output->second.inputBufferName.c_str());
}

const std::vector<Quotation>& Chart::GetData(const std::wstring& name)
{
	return this->data[name].data;
}

IChart* Chart::SetDataManipulator(IDataManipulator* obj, const wchar_t* inputBufferName, const wchar_t* outputBufferName)
{
	this->data[outputBufferName].manipulator = obj;
	this->data[outputBufferName].inputBufferName = inputBufferName;
	return this;
}
