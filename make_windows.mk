COMPILER_FLAGS = \
	-I "C:/local/include/"  \
	-FC -GL -GS -MP4 -O2 -Oi -W4 -WX -WL -Z7 -Zo -diagnostics:column \
	-fp:fast -nologo -sdl -guard:cf -permissive- -std:c11 \
	-wd4996 -wd4189

LINKER_FLAGS = \
	-LIBPATH:"C:/local/lib/" \
	/SUBSYSTEM:console \
	-CGTHREADS:8 -DEBUG:FULL -INCREMENTAL:no -opt:ref \
	libserialport.lib user32.lib gdi32.lib shell32.lib

.PHONY: main run clean

main:
	@echo "############################################################"
	@echo "### Building"
	@echo "############################################################"
	ctime -begin bin/gb01-ftdi-fixer.ctm
	cl $(COMPILER_FLAGS) \
		main.c \
		-Fo"bin/" \
		-link $(LINKER_FLAGS) \
		-out:bin/gb01-ftdi-fixer.exe
	ctime -end bin/gb01-ftdi-fixer.ctm %LastError%

run:
	@bin/gb01-ftdi-fixer.exe

clean:
	rm bin/*
