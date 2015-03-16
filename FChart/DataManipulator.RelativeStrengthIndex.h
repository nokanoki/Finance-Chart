#pragma once
#include "api.h"

namespace fchart
{
	namespace DataManipulator
	{
		class RelativeStrengthIndex
			: public IRelativeStrengthIndex
		{
		public:
			virtual void Calc(const Quotation* in, Quotation* out, const size_t& size) override;
		};
	}
}