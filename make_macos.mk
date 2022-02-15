COMPILER_FLAGS = \
	-I /usr/local/Cellar/libserialport/0.1.1/include/ \
	-D_FORTIFY_SOURCE=2 -ggdb3 -Og -Wall -Werror -Wextra -pedantic \
	-std=gnu11 \
	-Wno-unused-result

LINKER_FLAGS = \
	-L /usr/local/Cellar/libserialport/0.1.1/lib/ \
	-lserialport

.PHONY: main dist run clean

main:
	@echo "############################################################"
	@echo "### Building"
	@echo "############################################################"
	time gcc $(COMPILER_FLAGS) src/main.c -o bin/gb01-ftdi-fixer $(LINKER_FLAGS)

dist: main
	rm -rf dist/
	cp -r dist-template/macos dist/
	cp bin/gb01-ftdi-fixer dist/
	cp -r data dist/data
	mkdir -p dist/tools
	cp -r tools/macos-intel dist/tools/macos-intel
	cd dist && zip -r gb01-ftdi-fixer-macos-intel.zip *

run:
	@bin/gb01-ftdi-fixer

clean:
	rm bin/*
