#include "SystemInfo.h"
#include <thread>

using namespace std;

int SystemInfo::GetThreadCount()
{
	return thread::hardware_concurrency();
}
