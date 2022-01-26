ifeq ($(OS),Windows_NT)
	include make_windows.mk
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		$(error macOS not supported yet.)
	else
		$(error Linux not supported yet.)
	endif
endif
