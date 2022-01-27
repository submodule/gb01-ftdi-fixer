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

.PHONY: main dist run clean

main:
	@echo "############################################################"
	@echo "### Building"
	@echo "############################################################"
	ctime -begin bin/gb01-ftdi-fixer.ctm
	cl $(COMPILER_FLAGS) \
		main.c \
		libserialport.lib \
		-Fo"bin/" \
		-link $(LINKER_FLAGS) \
		-out:bin/gb01-ftdi-fixer.exe
	cp dlls/* bin/
	ctime -end bin/gb01-ftdi-fixer.ctm %LastError%

dist:
	rm -rf dist/
	mkdir -p dist/
	cp bin/gb01-ftdi-fixer.exe dist/
	cp dlls/* dist/
	cp -r data dist/data
	cp -r tools dist/tools

run:
	@bin/gb01-ftdi-fixer.exe

clean:
	rm bin/*
