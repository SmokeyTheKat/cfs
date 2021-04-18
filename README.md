# cfs
cfs is a simple package installer that compilers pacages from source. It uses general programs that can be install on most oses like make, gcc, and curl. A cfs just has to be hosted on a html server where the program can curl the data.

## usage
to use cfs you use
```
cfs -[options] [pkg name]
```

| options | action |
| ------: | ------ |
| f | fetchs data from repos |
| C | caches all repos |
| d | downloads selected packages |
| c | compiles selected packages |
| i | downloads and compiles selected packages |
| u | updates selected packages |
| r | removes selected packages |
| l | lists all packages from repos |
| L | prints raw data from all repos |
| h | help |

An examplse of downloading a program would be
```
cfs -fi mypkg
```
The 'f' here pull data from repos.
If you cache repos you do not need to fetch them.
Cache them again to update repos.

The active repos can be changed in the config.c file which then will need a recompile of the program for cnages to take place. As the program is small this will only take a few seconds.

## cfs package layout
Here is an example of a package
```
pkg:mypkg
dwl:mkdir -p %p%n
dwl:git clone https://github.com/mypackage/mypkg
inst:cd %p%n && make && sudo make install
up:cd %p%n && git pull
rm:cd %p%n && sudo make uninstall
rm:rm -rf %p%n
pkg:nextpackage
...
```
Packages can be put one after another in a repo.

The '%'s represent different data that can be used.
| Symbol | Data |
| -----: | ---- |
| %p | The folder where the program will be install to |
| %n | The name of the package |
