#include <main.h>

extern "C" void my_main()
{
	while(true)
	{
		[[maybe_unused]] volatile int dummy = 0;
	}
}
