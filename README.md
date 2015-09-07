LPC812 USB-GPIO module
======================

What is this?
-------------
This is a GPIO control module via USB-serial for NXP LPC812(LPC812M101JD20FP).

How to use
----------
Following tools are required to build and write firmware(hex file).
- NXP LPCXpresso
- Flash Magic

UART command list
-----------------
- help
	- show UART command list
- version
	- show firmware version
- gpio list
	- show all gpio information
- gpio mode $pin $mode
	- set gpio mode ($mode=0:Hi-Z, 1:pulldown, 2:pullup, 3:repeater)
- gpio dir $pin $dir
	- set gpio direction ($dir=0:input, 1:output)
- gpio read $pin
	- read gpio input value
- gpio write $pin $value
	- write gpio output value

License
-------
MIT-License
please refer to LICENSE file.

