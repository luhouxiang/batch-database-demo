.PHONY: default install test tags clean clean-all

default:
	make -C src

install:
	make -C src install

test: install
	make -C test

clean:
	make -C src clean
	rm -f bin/batch-database-demo

clean-all: clean
	make -C src clean-all
	rm -f log/batch-database-demo.log
