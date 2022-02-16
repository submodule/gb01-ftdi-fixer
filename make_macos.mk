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
	mkdir -p dist/tools
	cp -r tools/macos-intel dist/tools/macos-intel
	cp $$(find -L /usr/local/opt -name 'libftdi.1.dylib' 2>&-) dist/
	install_name_tool -change $$(find -L /usr/local/opt -name 'libftdi.1.dylib' 2>&-) @loader_path/../../libftdi.1.dylib dist/tools/macos-intel/ft232r_prog
	cp $$(find -L /usr/local/opt -name 'libusb-0.1.4.dylib' 2>&-) dist/
	install_name_tool -change $$(find -L /usr/local/opt -name 'libusb-0.1.4.dylib' 2>&-) @loader_path/../../libusb-0.1.4.dylib dist/tools/macos-intel/ft232r_prog
	cp $$(find -L /usr/local/opt -name 'libserialport.0.dylib' 2>&-) dist/
	install_name_tool -change $$(find -L /usr/local/opt -name 'libserialport.0.dylib' 2>&-) @loader_path/libserialport.0.dylib dist/gb01-ftdi-fixer
	cp $$(find -L /usr/local/opt/gcc -name 'libgcc_s.1.dylib' 2>&-) dist/
	install_name_tool -change $$(find -L /usr/local/opt/gcc -name 'libgcc_s.1.dylib' 2>&-) @loader_path/libgcc_s.1.dylib dist/gb01-ftdi-fixer
	cp $$(find -L /usr/local/opt -name 'libusb-1.0.0.dylib' 2>&-) dist/
	install_name_tool -change $$(find -L /usr/local/opt -name 'libusb-1.0.0.dylib' 2>&-) @loader_path/libusb-1.0.0.dylib dist/libusb-0.1.4.dylib
	cp -r data dist/data
	cd dist && zip -r gb01-ftdi-fixer-macos-intel.zip *

run:
	@bin/gb01-ftdi-fixer

clean:
	rm bin/*
