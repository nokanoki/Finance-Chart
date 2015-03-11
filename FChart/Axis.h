#pragma once
#include "Platform.h"
#include <map>
namespace fchart
{
	struct _HighlightStruct
	{
		int32_t brush;
		int32_t textFormat;
		float pos;
	};
	class Axis
	{
	public:
		Axis(IPlatform *pPlatform);
		virtual ~Axis();

		virtual void SetRect(const Rect& rc);

		virtual int32_t CreateHighlight(const float& pos, const int32_t& bgcolor);
		virtual void RemoveHighlight(const int32_t& id);
		virtual void SetHighlight(const int32_t& id, const float& pos);



		virtual void Draw();
		
	private:
		IPlatform *pPlatform;
		Rect rcAxis;
		int32_t textFormat;
		int32_t textBrush;
		int32_t lastId;

		std::map<int32_t, _HighlightStruct> highlights;
	};
}