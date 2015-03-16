#include "DataManipulator.Factory.h"

#include "DataManipulator.SMA.h"


using namespace fchart::DataManipulator;

Factory::~Factory()
{

}
ISMA* Factory::CreateSMA(const int32_t& val)
{
	return new SMA(val);
}