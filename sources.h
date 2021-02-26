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
		},
		{
			"cd %p%n && make uninstall",
			"sudo rm -rf %p%n",
			0, 0, 0, 0, 0, 0, 0, 0
		}
	},
	{ 
		"tty-snake", 
		{
			"mkdir %p%n",
			"git clone https://github.com/smokeythekat/tty-snake %p%n",
			0, 0, 0, 0, 0, 0, 0, 0
		},
		{
			"cd %p%n && make && sudo make install",
			0, 0, 0, 0, 0, 0, 0, 0, 0
		},
		{
			"sudo rm /usr/bin/tty-snake",
			"sudo rm -rf %p%n",
			0, 0, 0, 0, 0, 0, 0, 0
		}

	},
	{ 
		"tty-pong", 
		{
			"mkdir %p%n",
			"git clone https://github.com/smokeythekat/tty-pong %p%n",
			0, 0, 0, 0, 0, 0, 0, 0
		},
		{
			"cd %p%n && make && sudo make install",
			0, 0, 0, 0, 0, 0, 0, 0, 0
		},
		{
			"sudo rm /usr/bin/tty-pong",
			"sudo rm -rf %p%n",
			0, 0, 0, 0, 0, 0, 0, 0
		}

	},
};

#endif
