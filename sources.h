#ifndef __cfs_sources_h__
#define __cfs_sources_h__

#include "./src/utils.h"

struct package packages[] = {
	{ 
		"scroll", 
		{
			"mkdir %p%n",
			"git clone https://git.suckless.org/scroll %p%n",
			0, 0, 0, 0, 0, 0, 0, 0
		},
		{
			"cd %p%n && make && sudo make install",
			0, 0, 0, 0, 0, 0, 0, 0, 0
		}
	},
};

#endif
