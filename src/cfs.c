#include <ddcString.h>
#include <ddcPrint.h>

#include "./utils.h"
#include "./args.h"
#include "../config.h"
#include "../sources.h"

int package_get_com_list_length(char* command_list[10])
{
	int length = 0;
	while (command_list[++length] != 0);
	return length;
}

ddString package_generate_command(struct package pkg, char* comm_list[10], int index)
{
	const char* com = comm_list[index];
	ddString output = make_ddString("");
	long length;
	cstring_get_length(com, &length);
	for (int i = 0; i < length; i++)
	{
		switch (com[i])
		{
			case '%':
			{
				switch (com[i+1])
				{
					case 'p':
					{
						ddString_push_cstring_back(&output, programs_path);
					} break;
					case 'n':
					{
						ddString_push_cstring_back(&output, pkg.name);
					} break;
				}
				i++;
			} break;
			default: ddString_push_char_back(&output, com[i]);
		}
	}
	return output;
}

void package_remove(struct package pkg)
{
	for (int i = 0; i < package_get_com_list_length(pkg.remove_commands); i++)
	{
		ddString command = package_generate_command(pkg, pkg.remove_commands, i);
		ddPrints(command.cstr);
		ddPrint_nl();
		system(command.cstr);
		raze_ddString(&command);
	}
}
void package_download(struct package pkg)
{
	for (int i = 0; i < package_get_com_list_length(pkg.download_commands); i++)
	{
		ddString command = package_generate_command(pkg, pkg.download_commands, i);
		ddPrints(command.cstr);
		ddPrint_nl();
		system(command.cstr);
		raze_ddString(&command);
	}
}
void package_compile(struct package pkg)
{
	for (int i = 0; i < package_get_com_list_length(pkg.compile_commands); i++)
	{
		ddString command = package_generate_command(pkg, pkg.compile_commands, i);
		ddPrints(command.cstr);
		ddPrint_nl();
		system(command.cstr);
		raze_ddString(&command);
	}
}

struct package package_get_value(ddString name)
{
	for (int i = 0; i < sizeof(packages)/sizeof(struct package); i++)
		if (ddString_compare_cstring(name, packages[i].name)) return packages[i];
}

int main(int argc, char** argv)
{
	read_args(argc, argv);
	if (args_if_def(make_constant_ddString("-i")))
	{
		struct package pkg = package_get_value(args_get_value(make_constant_ddString("-i")));
		package_download(pkg);
		package_compile(pkg);
	}
	else if (args_if_def(make_constant_ddString("-d")))
	{
		struct package pkg = package_get_value(args_get_value(make_constant_ddString("-d")));
		package_download(pkg);
	}
	else if (args_if_def(make_constant_ddString("-c")))
	{
		struct package pkg = package_get_value(args_get_value(make_constant_ddString("-c")));
		package_compile(pkg);
	}
	else if (args_if_def(make_constant_ddString("-r")))
	{
		struct package pkg = package_get_value(args_get_value(make_constant_ddString("-r")));
		package_remove(pkg);
	}
	return 0;
}
