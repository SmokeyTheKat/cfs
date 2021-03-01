#include <stdio.h>
#include <ddcString.h>
#include <ddcPrint.h>

#include "./utils.h"
#include "./args.h"
#include "../config.h"

struct package pkgs[100];
long pkg_count = -1;

int package_get_com_list_length(char* command_list[10])
{
	int length = 0;
	while (command_list[++length] != 0);
	return length;
}

ddString package_generate_command(struct package pkg, char* comm_list[10], int index)
{
	const char* com = comm_list[index];
	ddString output = make_ddString_capacity("", 100);
	long length;
	cstring_get_length(com, &length);
	for (int i = 0; i < length; i++)
	{
		ddPrint_char(com[i]);
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
	for (int i = 0; i < pkg.remove_count; i++)
	{
		ddString command = package_generate_command(pkg, pkg.remove_commands, i);
		system(command.cstr);
		raze_ddString(&command);
	}
}
void package_download(struct package pkg)
{
	for (int i = 0; i < pkg.download_count; i++)
	{
		ddString command = package_generate_command(pkg, pkg.download_commands, i);
		system(command.cstr);
		raze_ddString(&command);
	}
}
void package_compile(struct package pkg)
{
	for (int i = 0; i < pkg.compile_count; i++)
	{
		ddString command = package_generate_command(pkg, pkg.compile_commands, i);
		system(command.cstr);
		raze_ddString(&command);
	}
}

struct package package_get_value(ddString name)
{
	for (int i = 0; i < pkg_count; i++)
	{
		if (cstring_compare(name.cstr, pkgs[i].name))
		{
			return pkgs[i];
		}
	}
	ddPrintf("ERROR PACKAGE NOT FOUND\n");
	exit(1);
}

int main(int argc, char** argv)
{
	read_args(argc, argv);

	if (args_if_def(make_constant_ddString("-l")) ||
		args_if_def(make_constant_ddString("-la")))
	{
		for (int i = 0; i < sizeof(sources)/sizeof(char*); i++)
		{
			printf("SOURCE %s\n", sources[i]);

			FILE *fp;
			char path[1035];

			ddString command = make_format_ddString("curl -sS %s", sources[i]);

			fp = popen(command.cstr, "r");

			raze_ddString(&command);

			if (fp == NULL)
			{
				printf("Failed to run command\n" );
				exit(1);
			}


			while (fgets(path, sizeof(path), fp) != NULL)
			{
				if (args_if_def(make_constant_ddString("-la"))) ddPrints(path);
				if (cstring_compare_length("pkg:", path, 4))
					printf(path+4);
			}
			pclose(fp);
		}
		exit(1);
	}

	for (int i = 0; i < sizeof(sources)/sizeof(char*); i++)
	{
		FILE *fp;
		char path[1035];

		ddString command = make_format_ddString("curl -sS %s", sources[i]);

		fp = popen(command.cstr, "r");

		raze_ddString(&command);

		if (fp == NULL)
		{
			printf("Failed to run command\n" );
			exit(1);
		}


		while (fgets(path, sizeof(path), fp) != NULL)
		{
			if (cstring_compare_length("pkg:", path, 4))
			{
				pkg_count++;
				ddString v = make_ddString(path+4);
				v.cstr[v.length-1] = 0;
				pkgs[pkg_count].name = v.cstr;
				//ddPrints(pkgs[pkg_count].name);
			}
			else if (cstring_compare_length("inst:", path, 5))
			{
				ddString v = make_ddString(path+5);
				v.cstr[v.length-1] = 0;
				pkgs[pkg_count].compile_commands[pkgs[pkg_count].compile_count++] = v.cstr;
				//ddPrints(pkgs[pkg_count].compile_commands[pkgs[pkg_count].compile_count-1]);
			}
			else if (cstring_compare_length("dwl:", path, 4))
			{
				ddString v = make_ddString(path+4);
				v.cstr[v.length-1] = 0;
				pkgs[pkg_count].download_commands[pkgs[pkg_count].download_count++] = v.cstr;
				//ddPrints(pkgs[pkg_count].download_commands[pkgs[pkg_count].download_count-1]);
			}
			else if (cstring_compare_length("rm:", path, 3))
			{
				ddString v = make_ddString(path+3);
				v.cstr[v.length-1] = 0;
				pkgs[pkg_count].remove_commands[pkgs[pkg_count].remove_count++] = v.cstr;
				//ddPrints(pkgs[pkg_count].remove_commands[pkgs[pkg_count].remove_count-1]);
			}
		}
		pclose(fp);
	}
	pkg_count++;

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
