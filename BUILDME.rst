MIN Echo
********

**MIN protocol demo. STM32F103C8T6, STM32CubeIDE, USB VCP**

Flash release build
===================

- On ST-Link disconnected from the target:

::

    $ lsusb

- Connect to the target and check:

::

    $ st-info --probe
    ...
    $ st-flash --reset write Release/minecho.bin 0x08000000
