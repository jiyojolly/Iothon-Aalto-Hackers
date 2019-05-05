# Using GCC to program MKR NB 1500

These instructions are work in progress.

At this writing these are for Mac OS X only.  Feel free to make a PR for Windows or Linux.

## Understanding the Arduino IDE runtime

At Mac OS X, the Arduino IDE stores its runtime to
```
  $HOME/Library/Arduino15/packages/arduino/hardware/samd/<version>
```
where the current version at this writing is
```
  $HOME/Library/Arduino15/packages/arduino/hardware/samd/1.6.21
```

See the following web pages for the required background information:
* [Arduino IDE Build process](https://github.com/arduino/Arduino/wiki/Build-Process)
* [Arduino IDE 3rd party Hardware Spec](https://github.com/arduino/Arduino/wiki/Arduino-IDE-1.5-3rd-party-Hardware-specification)

Quite obviously, the folder layout follows the hardware specification.
