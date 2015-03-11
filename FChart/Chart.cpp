#include "Chart.h"

using namespace fchart;


Chart::Chart(IPlatform *platform)
	:pPlatform(platform)
	, testArea(pPlatform)
{
	platform->AddRef();
	testArea.SetRect(makerect(100, 100, 50, 10));
	testArea.SetBackground(0xff000099);
	platform->AddEventMouseMove(&testArea);
	platform->AddEventMouseMove(this);
}
Chart::~Chart()
{
	pPlatform->Release();
}
void Chart::SetSize(const int32_t& width, const int32_t& height)
{
	this->pPlatform->SetSize(width, height);
	this->testArea.SetRect(makerect(100.f, height -100.f, width - 100.f, 100.f ));
	this->Render();
}




void Chart::Render()
{
	this->pPlatform->Begin();
	this->testArea.Draw();
	this->pPlatform->End();
}

void Chart::OnMouseMove(const MouseEventArgs& e)
{
	this->Render();
}