#ifndef __cfs_utils_h__
#define __cfs_utils_h__

struct package
{
	char* name;
	int download_count;
	char* download_commands[10];
	int update_count;
	char* update_commands[10];
	int compile_count;
	char* compile_commands[10];
	int remove_count;
	char* remove_commands[10];
};

#endif
