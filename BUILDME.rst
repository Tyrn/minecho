Galvanix
********

- `Eclipse setup and misc <https://github.com/Tyrn/arch-chronicle/blob/master/Usage/Ac6_SW4STM32.md>`__

Run unit tests on host
======================

- `Ceedling <https://github.com/ThrowTheSwitch/Ceedling>`__
- Create testing project (once, at the very beginning):

::

    galvanix $ cd ..
    workspace $ ceedling new galvanix
    workspace $ cd galvanix
    ...

All you need is ``project.yml``, to be put on VCS (Git).
To run from *Eclipse*, make sure to replace ``stdout_pretty_tests_report``
with ``stdout_gtestlike_tests_report``.

- Run from command line:

::

    $ ceedling help
    $ ceedling clobber
    $ ceedling test:all

Flash release build
===================

- On ST-Link disconnected from the target:

::

    $ lsusb

- Connect to the target and check:

::

    $ st-info --probe
    ...
    $ st-flash --reset write Release/galvanix.bin 0x08000000
