#ifndef __GENERIC_H
#define __GENERIC_H

#ifdef WIN32
#include <conio.h>
#else
#include <unistd.h>
#endif

template <class T>
class TGeneric : public T
{
private:
	int count;
public:
	TGeneric()
	: T()
	{
		count = 0;
	}
	virtual ~TGeneric()
	{
	}
	virtual void OnStep()
	{
		const int divs = 0x200;
		char c, *steps = "/-\\|";
		c = steps[(count++%(4*divs))/divs];
		if (!(count%divs))
		{
			printf("%c\tStep\t%d\r", c, count);
#ifdef WIN32
			if (kbhit() && (getch() == 27))
			{
				printf("\nProcess stopped by user...              \n");
				exit(1);
			}
#else
			usleep(1);
#endif
		}
	}
};

#endif
