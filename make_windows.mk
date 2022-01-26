COMPILER_FLAGS = \
	-FC -GL -GS -MP4 -O2 -Oi -W4 -WX -WL -Z7 -Zo -diagnostics:column \
	-fp:fast -nologo -sdl -guard:cf -permissive- -std:c11 \
	-wd4996

LINKER_FLAGS = \
	/SUBSYSTEM:console \
	-CGTHREADS:8 -DEBUG:FULL -INCREMENTAL:no -opt:ref \
	user32.lib gdi32.lib shell32.lib

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
