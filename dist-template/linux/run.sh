#!/bin/sh -e

printf 'You will now be prompted for superuser rights in order to obtain sufficient\n'
printf 'permissions to access serial ports.\n\n'

sudo LD_LIBRARY_PATH=.:${LD_LIBRARY_PATH} ./gb01-ftdi-fixer
