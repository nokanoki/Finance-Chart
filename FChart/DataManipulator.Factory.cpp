#include "DataManipulator.Factory.h"

#include "DataManipulator.SMA.h"
#include "DataManipulator.RelativeStrengthIndex.h"


using namespace fchart::DataManipulator;

Factory::~Factory()
{

}
ISMA* Factory::CreateSMA(const int32_t& val)
{
	return new SMA(val);
}
IRelativeStrengthIndex* Factory::CreateRSI()
{
	return new RelativeStrengthIndex;
}
