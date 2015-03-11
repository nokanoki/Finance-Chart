#include "Axis.h"
using namespace fchart;

Axis::Axis(IPlatform *pPlatform) :
pPlatform(pPlatform), lastId(0)
{
	pPlatform->AddRef();
	this->textFormat = pPlatform->CreateTextFormat(maketextprps());
	//this->textBrush = pPlatform->CreateBrush(makesolidbrushprps(0xffffffff));
	

}
Axis::~Axis()
{
	pPlatform->Release();
	for (auto h : this->highlights)
	{
		this->pPlatform->RemoveBrush(h.second.brush);
	}
}

void Axis::SetRect(const Rect& rc)
{
	this->rcAxis = rc;
}

int32_t Axis::CreateHighlight(const float& pos, const int32_t& bgcolor)
{
	lastId++;
	_HighlightStruct h;
	//h.brush = pPlatform->CreateBrush(makesolidbrushprps(bgcolor));
	//this->highlights[lastId] = h;
	return lastId;
}
void Axis::RemoveHighlight(const int32_t& id)
{
	auto find = this->highlights.find(id);
	if (find != this->highlights.end())
	{
		this->pPlatform->RemoveBrush(find->second.brush);
		this->highlights.erase(find);
	}
}
void Axis::SetHighlight(const int32_t& id, const float& pos)
{
	auto find = this->highlights.find(id);
	if (find != this->highlights.end())
		find->second.pos = pos;
}



void Axis::Draw()
{
	float height = rcAxis.bottom - rcAxis.top;
	float count = height / 20.f;
	this->pPlatform->SetTextFormat(this->textFormat);
	this->pPlatform->SetBrushOutline(this->textBrush);
	for (float i = rcAxis.top; i < height; i+= 20.f)
	{
		Point p = makepoint(rcAxis.left, i+10);
		pPlatform->DrawText(p, std::to_wstring(i).c_str());
	}
	for (auto h : this->highlights)
	{
		pPlatform->SetBrushFill(h.second.brush);
		pPlatform->ClearRect(makerect(rcAxis.left, h.second.pos + 10.f, rcAxis.right, h.second.pos - 10.f));
		pPlatform->DrawText(makepoint(rcAxis.left, h.second.pos + 10.f), std::to_wstring(h.second.pos).c_str());
	}
	
}
