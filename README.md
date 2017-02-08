Description
======

A example project for libopencm3 standalone. And it has support for dapboot bootloader to support USB downloading.

Hardware requirement and IO assignment
--------------

It requires a STM32F103C8T6 or equal microcontroller(GD32/MM32/BLM32) with 8MHz external crystal.

PB12 - LED

Build
--------------

Enter the src directory, and type "make"

Flash
--------------

This firmware requires a bootloader which called dapboot.

Try my branch https://github.com/rgwan/dapboot, target is "MSC".

And use "make dfu" to flash your microcontroller via USB DFU protocol.

