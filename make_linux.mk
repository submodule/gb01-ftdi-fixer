COMPILER_FLAGS = \
	-I "C:/local/include/"  \
	-MT -FC -GL -GS -MP4 -O2 -Oi -W4 -WX -WL -Z7 -Zo -diagnostics:column \
	-fp:fast -nologo -sdl -guard:cf -permissive- -std:c11 \
	-wd4996 -wd4189 -wd4102 -wd4100

LINKER_FLAGS = \
	-LIBPATH:"C:/local/lib/libserialport-release" \
	/SUBSYSTEM:console \
	-CGTHREADS:8 -DEBUG:FULL -INCREMENTAL:no -opt:ref \
	user32.lib gdi32.lib shell32.lib

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

dist:
	rm -rf dist/
	mkdir -p dist/
	cp bin/gb01-ftdi-fixer dist/
	cp -r data dist/data
	mkdir -p dist/tools
	cp -r tools/linux dist/tools/linux

run:
	@bin/gb01-ftdi-fixer

clean:
	rm bin/*
