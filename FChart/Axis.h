#pragma once
#include "Platform.h"
#include "api.h"
//#include "ChartArea.h"
#include <map>
#include <vector>
namespace fchart
{
	
	

	class Axis
		: public IAxis, public virtual Object
	{
	public:
		Axis(IPlatform *pPlatform, const AxisType& type);
		virtual ~Axis();

		virtual void SetRect(const Rect& rc);
		virtual void SetTransformation(const Transformation& trans);
		/*TEST*/
		virtual void SetDataType(const AxisDataType& type);
		virtual void SetData(const std::vector<Quotation>& data);

		virtual void Draw();
		
	private:
		void DrawVertical();
		void DrawHorizontal();

	private:
		IPlatform *pPlatform;
		Rect rcAxis;
		ITextFormat *pTextFormat;
		IBrush *pTextBrush;
		Transformation transformation;
		AxisType axisType; 
		AxisPosition axisPosition;
		Rect rcUserDefine;
		AxisDataType axisDataType;
		
		float lblFactor;
		float gridFactor;

		/*TEST*/
		float dataPointWidth;
		std::vector<Quotation> data;
	};
}