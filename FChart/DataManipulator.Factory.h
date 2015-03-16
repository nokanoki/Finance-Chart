#pragma once
#include "api.h"
#include "Object.h"
namespace fchart
{
	namespace DataManipulator
	{
		class Factory
			: public IFactory, public Object
		{
		public:
			virtual ~Factory() override;
			virtual ISMA* CreateSMA(const int32_t& val) override;
		};
	}
}