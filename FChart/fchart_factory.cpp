#include "api.h"
#include "Chart.h"
#include "DataManipulator.Factory.h"

#if FCHART_COMPILE_PLATFORM_WIN_D2D1
#include "DirectX2DPlatform.h"
#endif

int fchart_factory(FCHART_FACTORY_STRUCT *pStruct)
{
	if (pStruct->id == FCHART_FACTORY_ID_CHART)
	{
#if FCHART_COMPILE_PLATFORM_WIN_D2D1

		fchart::IPlatform *platform = new fchart::PlatformD2D1((HWND)pStruct->hWnd,
			pStruct->x, pStruct->y,
			pStruct->width, pStruct->height);

		*pStruct->ppOut = reinterpret_cast<void**>(new fchart::Chart(platform, pStruct->width, pStruct->height));
		platform->Release();
		return 0;
#else
		return 1;// not imp
#endif
	}
	else if (pStruct->id == FCHART_FACTORY_ID_DATAMANIMUPATOR_FACTORY)
	{
		*pStruct->ppOut = reinterpret_cast<void*>(new fchart::DataManipulator::Factory);
		return 0;
	}
	return 1;
}