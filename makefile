all:
	gcc ./src/cfs.c -o ./cfs
install:
	cp ./cfs /usr/bin/
