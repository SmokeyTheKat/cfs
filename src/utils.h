#ifndef __cfs_utils_h__
#define __cfs_utils_h__

struct package
{
	char* name;
	char* download_commands[10];
	char* compile_commands[10];
	char* remove_commands[10];
};

#endif
