COMPILER_FLAGS = \
	-D_FORTIFY_SOURCE=2 -ggdb3 -Og -Wall -Werror -Wextra -pedantic \
	-std=gnu11 \
	-Wno-unused-result

LINKER_FLAGS = \
	-lserialport

.PHONY: main dist run clean

main:
	@echo "############################################################"
	@echo "### Building"
	@echo "############################################################"
	time gcc $(COMPILER_FLAGS) src/main.c -o bin/gb01-ftdi-fixer $(LINKER_FLAGS)

dist: main
	rm -rf dist/
	cp -r dist-template/linux dist/
	cp bin/gb01-ftdi-fixer dist/
	mkdir -p dist/tools
	cp -r tools/linux64 dist/tools/linux64
	cp $$(find /lib /usr/lib -name 'libftdi.so.1') dist/
	cp $$(find /lib /usr/lib -name 'libserialport.so.0') dist/
	cp $$(find /lib /usr/lib -name 'libusb-0.1.so.4') dist/
	cp -r data dist/data
	cd dist && zip -r gb01-ftdi-fixer-linux64.zip *

run:
	@bin/gb01-ftdi-fixer

clean:
	rm bin/*
