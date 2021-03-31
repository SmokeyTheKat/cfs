#include <stdio.h>
#include <ddcString.h>
#include <ddcPrint.h>
#include <ddcDef.h>
#include <ddcPrint.h>

#include "./utils.h"
#include "../config.h"

struct package pkgs[100];
long pkg_count = -1;

void print_help(void)
{
	ddPrintf("cfs\n");
	ddPrintf("usage: cfs -[options] package\n");
	ddPrintf("options:\n");
	ddPrintf("	f	-	fetch from sources\n");
	ddPrintf("	d	-	download package\n");
	ddPrintf("	c	-	compile package\n");
	ddPrintf("	i	-	download + compile package\n");
	ddPrintf("	u	-	update package\n");
	ddPrintf("	r	-	remove package\n");
	ddPrintf("	l	-	list packages\n");
	ddPrintf("	L	-	list packages raw\n");
	ddPrintf("	h	-	help\n");
	ddPrintf("	h	-	help\n");
	ddPrintf("docs: http://ddmo.xyz/projects/cfs/\n");
}

struct package get_package(ddString name)
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
					case 'x':
					{
						ddString_push_cstring_back(&output, prefix);
					} break;
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

void package_remove(int argc, char** argv)
{
	for (int i = 2; i < argc; i++)
	{
		struct package pkg = get_package(make_constant_ddString(argv[i]));
		for (int j = 0; j < pkg.remove_count; j++)
		{
			ddString command = package_generate_command(pkg, pkg.remove_commands, j);
			system(command.cstr);
			raze_ddString(&command);
		}
	}
}
void package_update(int argc, char** argv)
{
	for (int i = 2; i < argc; i++)
	{
		struct package pkg = get_package(make_constant_ddString(argv[i]));
		for (int j = 0; j < pkg.update_count; j++)
		{
			ddString command = package_generate_command(pkg, pkg.update_commands, j);
			system(command.cstr);
			raze_ddString(&command);
		}
	}
}
void package_download(int argc, char** argv)
{
	for (int i = 2; i < argc; i++)
	{
		struct package pkg = get_package(make_constant_ddString(argv[i]));
		for (int j = 0; j < pkg.download_count; j++)
		{
			ddString command = package_generate_command(pkg, pkg.download_commands, j);
			system(command.cstr);
			raze_ddString(&command);
		}
	}
}
void package_compile(int argc, char** argv)
{
	for (int i = 2; i < argc; i++)
	{
		struct package pkg = get_package(make_constant_ddString(argv[i]));
		for (int j = 0; j < pkg.compile_count; j++)
		{
			ddString command = package_generate_command(pkg, pkg.compile_commands, j);
			system(command.cstr);
			raze_ddString(&command);
		}
	}
}

void fetch_packages(void)
{
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
			}
			else if (cstring_compare_length("inst:", path, 5))
			{
				ddString v = make_ddString(path+5);
				v.cstr[v.length-1] = 0;
				pkgs[pkg_count].compile_commands[pkgs[pkg_count].compile_count++] = v.cstr;
			}
			else if (cstring_compare_length("up:", path, 3))
			{
				ddString v = make_ddString(path+3);
				v.cstr[v.length-1] = 0;
				pkgs[pkg_count].update_commands[pkgs[pkg_count].update_count++] = v.cstr;
			}
			else if (cstring_compare_length("dwl:", path, 4))
			{
				ddString v = make_ddString(path+4);
				v.cstr[v.length-1] = 0;
				pkgs[pkg_count].download_commands[pkgs[pkg_count].download_count++] = v.cstr;
			}
			else if (cstring_compare_length("rm:", path, 3))
			{
				ddString v = make_ddString(path+3);
				v.cstr[v.length-1] = 0;
				pkgs[pkg_count].remove_commands[pkgs[pkg_count].remove_count++] = v.cstr;
			}
		}
		pclose(fp);
	}
	pkg_count++;
}

int main(int argc, char** argv)
{
	if (argc == 1)
	{
		print_help();
		exit(0);
	}
	if (argc == 2 && (cstring_compare(argv[1], "--help") ||
			  cstring_compare(argv[1], "-help")))
	{
		print_help();
		exit(0);
	}

	char* comms = argv[1]+1;
	long commslen;
	cstring_get_length(comms, &commslen);


	for (int j = 0; j < commslen; j++)
	{
		if (comms[j] == 'f')
		{
			fetch_packages();
		}
		if (comms[j] == 'i')
		{
			package_download(argc, argv);
			package_compile(argc, argv);
		}
		else if (comms[j] == 'd')
		{
			package_download(argc, argv);
		}
		else if (comms[j] == 'u')
		{
			package_update(argc, argv);
			package_compile(argc, argv);
		}
		else if (comms[j] == 'c')
		{
			package_compile(argc, argv);
		}
		else if (comms[j] == 'r')
		{
			package_remove(argc, argv);
		}
		else if (comms[j] == 'L')
		{
			for (int k = 0; k < pkg_count; k++)
			{
				struct package pkg = pkgs[k];
				struct __pkg_data { int cnt; char* coms[10]; }*_pkg_data;
				_pkg_data = (struct __pkg_data*)((char*)&pkg.download_count);
				printf("%s\n", pkg.name);
				for (int i = 0; i < 4; i++)
				{
					for (int l = 0; l < _pkg_data[i].cnt; l++)
					{
						printf("    %s\n", _pkg_data[i].coms[l]);
					}
				}
			}
		}
		else if (comms[j] == 'l')
		{
			for (int k = 0; k < pkg_count; k++)
			{
				ddPrintf("%s\n", pkgs[k].name);
			}
		}
		else if (comms[j] == 'h')
		{
			print_help();
			exit(0);
		}
	}
	return 0;
}
