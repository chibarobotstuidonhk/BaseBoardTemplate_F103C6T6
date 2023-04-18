#include <main.h>

extern "C" void mymain()
{
	while(true)
	{
		[[maybe_unused]] volatile int dummy = 0;
	}
}
