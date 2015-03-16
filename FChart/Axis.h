#pragma once
#include "Platform.h"
#include "api.h"

#include <map>
#include <vector>

namespace fchart
{
	class ChartArea;

	class Axis
		: public IAxis
	{
	public:
		Axis() = delete;
		Axis(const Axis&) = delete;

		Axis(IPlatform *pPlatform, const AxisType& type, ChartArea* chartArea);
		~Axis();

		void SetRect(const Rect& rc);
		const Rect& GetLabelRect();
		void SetTransformation(const Transformation& trans);
		float GetAxisSize();
		virtual void Draw(const std::vector<Quotation>& data);
		std::wstring GetBufferSource();
		const AxisType& GetAxisType();

		//////////////  interface imp
	public:
		virtual IAxis* SetDataType(const AxisDataType& type);
		virtual IAxis* SetBufferSource(const wchar_t* name) override; 
		
		
	private:
		void DrawVertical(const std::vector<Quotation>& data);
		void DrawHorizontal(const std::vector<Quotation>& data);

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
		float dataPointWidth;
		ISeries *sourceSeries;
		std::wstring bufferName;
	};
}