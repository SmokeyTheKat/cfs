PREFIX=~/.local
all:
	gcc ./src/cfs.c -o ./cfs
install:
	cp ./cfs $(PREFIX)/bin/
uninstall:
	rm $(PREFIX)/bin/cfs
