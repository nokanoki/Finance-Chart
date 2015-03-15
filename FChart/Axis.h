#pragma once
#include "Platform.h"
#include "api.h"

#include <map>
#include <vector>

namespace fchart
{
	class ChartArea;

	class Axis
		: public IAxis, public virtual Object
	{
	public:
		Axis(IPlatform *pPlatform, const AxisType& type, ChartArea* chartArea);
		virtual ~Axis();
		virtual IAxis* SetSourceSeries(const wchar_t* name) override;
		virtual void SetRect(const Rect& rc);
		const Rect& GetLabelRect();
		virtual void SetTransformation(const Transformation& trans);
		/*TEST*/
		virtual IAxis* SetDataType(const AxisDataType& type);
		virtual const AxisType& GetAxisType();
		virtual float GetAxisSize();
		

		virtual void Draw();
		
	private:
		void DrawVertical();
		void DrawHorizontal();

	private:
		IPlatform *pPlatform;
		Rect rcAxis;
		Rect rcLabel;
		ITextFormat *pTextFormat;
		IBrush *pTextBrush;
		Transformation transformation;
		AxisType axisType; 
		AxisPosition axisPosition;
		Rect rcUserDefine;
		AxisDataType axisDataType;
		ChartArea *chartArea;
		
		float lblFactor;
		float gridFactor;
		float axisSize;

		/*TEST*/
		float dataPointWidth;
		ISeries *sourceSeries;
	};
}