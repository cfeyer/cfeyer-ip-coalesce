.PHONY: clean 

all:
	make -C src
	make -C test
	cd test; make check

clean:
	make -C src clean
	make -C test clean
