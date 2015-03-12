#pragma once
#include "Platform.h"
#include <map>
namespace fchart
{
	
	
	enum class AxisType{ Horizontal, Vertical };
	enum class AxisPosition { Left, Top, Right, Bottom, UserDefine };
	struct AxisProperties
	{
		AxisType type;
		AxisPosition position;
		Rect rcUserDefine;
		float lblFactor;
		float gridFactor;
	};
	__inline AxisProperties makeaxiesproperties(AxisType type){
		AxisProperties p;
		p.type = type;
		p.position = type == AxisType::Vertical ? AxisPosition::Right : AxisPosition::Bottom;
		p.lblFactor = 20.f;
		return p;
	}


	class Axis
	{
	public:
		Axis(IPlatform *pPlatform, const AxisProperties& props);
		virtual ~Axis();

		virtual void SetRect(const Rect& rc);
		virtual void SetTransformation(const Transformation& trans);
		
		
		virtual void Draw();
		
	private:
		void DrawVertical();


	private:
		IPlatform *pPlatform;
		Rect rcAxis;
		ITextFormat *pTextFormat;
		IBrush *pTextBrush;
		AxisProperties properties;
		Transformation transformation;
		
	};
}