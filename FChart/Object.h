#pragma once
#include "api.h"
namespace fchart
{
	class Object :
		public virtual fchart::IObject
	{
	public:
		Object();
		~Object() override;
		void Release() override;
		void AddRef() override;
	private:
		int refcount;
	};
}