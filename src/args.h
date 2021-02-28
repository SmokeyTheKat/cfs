#ifndef __cfs_args_h__
#define __cfs_args_h__

#include <ddcDef.h>
#include <ddcPrint.h>

struct compilerArgs
{
	ddString name;
	ddString value;
};

struct compilerArgs* cargs;
long cargsCount = 0;

void read_args(int argc, char** argv)
{
	if (argc == 1)
	{
		ddPrint_cstring("cfs\n");
		ddPrint_cstring("usage: cfs [OPTIONS...] name\n");
		ddPrint_cstring("options:\n");
		ddPrint_cstring("	-i			install package\n");
		ddPrint_cstring("	-c			compile/recompile package\n");
		ddPrint_cstring("	-d			dowload package\n");
		ddPrint_cstring("	-r			remove package\n");
		ddPrint_cstring("docs: http://ddmo.xyz/projects/cfs/\n");
		exit(0);
	}
	cargs = make(struct compilerArgs, 1000);
	for (int i = 1; i < argc; i++)
	{
		ddString dsarg = make_ddString(argv[i]);
		if (ddString_compare_cstring(dsarg, "--help") || ddString_compare_cstring(dsarg, "-help"))
		{
			ddPrint_cstring("cfs\n");
			ddPrint_cstring("usage: cfs [OPTIONS...] name\n");
			ddPrint_cstring("options:\n");
			ddPrint_cstring("	-i			install package\n");
			ddPrint_cstring("	-c			compile/recompile package\n");
			ddPrint_cstring("	-d			dowload package\n");
			ddPrint_cstring("	-r			remove package\n");
			ddPrint_cstring("docs: http://ddmo.xyz/projects/cfs/\n");
			exit(0);
		}
		else if (ddString_compare_cstring(dsarg, "-la"))
		{
			cargs[cargsCount++].name = dsarg;
		}
		else if (ddString_compare_cstring(dsarg, "-l"))
		{
			cargs[cargsCount++].name = dsarg;
		}
		else if (ddString_compare_cstring(dsarg, "-i"))
		{
			cargs[cargsCount].name = dsarg;
			cargs[cargsCount++].value = make_ddString(argv[++i]);
		}
		else if (ddString_compare_cstring(dsarg, "-c"))
		{
			cargs[cargsCount].name = dsarg;
			cargs[cargsCount++].value = make_ddString(argv[++i]);
		}
		else if (ddString_compare_cstring(dsarg, "-d"))
		{
			cargs[cargsCount].name = dsarg;
			cargs[cargsCount++].value = make_ddString(argv[++i]);
		}
		else if (ddString_compare_cstring(dsarg, "-r"))
		{
			cargs[cargsCount].name = dsarg;
			cargs[cargsCount++].value = make_ddString(argv[++i]);
		}
		else if (dsarg.cstr[0] != '-')
		{
			cargs[cargsCount].name = make_constant_ddString("__INPUT_FILE");
			cargs[cargsCount++].value = dsarg;
		}
	}
}
ddString args_get_value(ddString name)
{
	for (long i = 0; i < cargsCount; i++)
	{
		if (ddString_compare(name, cargs[i].name)) return cargs[i].value;
	}
	return make_constant_ddString("ERROR");
}
bool args_if_def(ddString name)
{
	for (long i = 0; i < cargsCount; i++)
	{
		if (ddString_compare(name, cargs[i].name)) return 1;
	}
	return 0;
}

#endif
