# Submodule FTDI Fixer

This is a tool to fix FTDI information on the Submodule GB01.

## Building

```
make
```

You might have to edit `make_*.mk` to add your own paths etc.

This program has the following dependencies:

* `libftdi0` (not the 1.x.x version)
* `libserialport`
* `libusb`
