# Arduino MKR NB 1500 MQTT sketch

The `mkr_nb_1500_mqtt` subfolder in this folder contains the
MKR NB 1500 sketch for using MQTT to publish data atMosquitto or
some other MQTT server.

The default settings are for the IoThon hackathon 2019 cloud
setup (see the other parts of this repository), and should work
without any changes.

By default the sketch publishes the temperature and signal
strength (RSSI) as measured by the NB-IoT modem on the MKR board.
**NOTE!** At this writing, the modem unit I am testing does
not report any temperature, and the RSSI is always `99`, which
indicates an error value.

## Known bugs and issues

* The software stack does not (usually) recover from closing the MQTT socket.
* Sometimes connecting to the NB-IoT network takes a long time (minutes).
  * Apparently sometimes this may take a **very** long time, or indefinitely.
* Reseting MKR NB 1500 with the reset button resets the USB connection.
* A busy loop in your software may appear to brick the device.
  * Pressing the reset button while trying to reprogram the device usually helps.

## Setup

### Installation

The easiest way to work with this software is to use the
[Arduino IDE](https://www.arduino.cc/en/Main/Software)
to compile and flash the sketch:

1. Download, install, and launch the IDE, following the
[instructions](https://www.arduino.cc/en/Guide/HomePage).

(The online version has not been tested and is somewhat unlikely to work.)

2. Install the PubSubClient library into the IDE

 a) Select Tools->Manage Libaries...
 b) Type "PubSubClient" to the "Filter your search..." box
 c) Scroll down to find "PubSubClient by Nick O'Leary"
 d) Click install

3. Install our custom version of the MKRNB library from github

We have our custom, enhanced version of the MKRNB Arduino
library at [IoThon github](https://github.com/iothon/MKRNB).

If you are not good with the command line, you can
just download a TAR archive and install it, following
[these helpful instructions](https://www.baldengineer.com/installing-arduino-library-from-github.html).

If you are happy with `git` on the command line, the
easiest way is just to clone the repo into the IDE libraries:

```sh
  cd ~/Documents/Arduino/libraries
  git clone https://github.com/iothon/MKRNB.git
```

If you cannot get the custom version working, you can
install the standard version, but then you have to modify
the sketch a little bit to remove unsupported functionality.

### Setting up

1. Connect your MKR NB 1500 with an USB cable.
2. Open the sketch in the IDE from "File->Open..."
3. Select "Arduino MKR NB 1500" from "Tools->Boards..."
4. Select "Arduino MKR NB 1500" from "Tools->Port..."
5. Test that the board works with "Tools->Get Board Info"
6. Check that the "Tools->Programmer" is "AVRISP mkll"

### Flashing and running

Once you have your sketch in the IDE and the board configured,
you are ready to flash and test:

1. Select "Sketch->Upload"
2. Wait until it is done and you see red "CPU reset" at the bottom of the windo.
3. Open "Tools->Serial Monitor" to see the info messages from the board.

### Debugging

To debug, modify the Sketch so that the `nbAccess` constructor has a `true`
instead of `false` as its parameter:
```C++
  NB nbAccess(true); // NB access: use a 'true' parameter to enable debugging
```

Then recompile and upload the sketch.  After the reflashing, you will
see how the sketch communicates with the modem.

## Definition of done

If you have now your cloud up and running as well, you should see the
temperature and RSSI data getting entered into your InfluxDB by the
`mqttbridge`, making it easy to get it visualised in Grafana.

Until you are there, feel free to ask help with debugging.

Once your data is being visualised, you are on your own to innovate.

At this writing, the sketch works for some 10 minutes, and then something
closes the socket.  The MKRNB library does not recognize this properly,
and fails to reconnect to the MQTT server.  We are working on fixing this
on our custom version of the MKRNB library, hoping to fix this before
the hackathon starts.

At this writing, the only way to recover is to reflash the sketch.

## Next steps

Once you have you initial infrastructure up and running, you most
probably want to add some cool sensors to your Arduino.  For that,
have a look at the following:
* [QWIIC Connect System](https://www.sparkfun.com/qwiic)
* [QWIIC tutorials page](https://learn.sparkfun.com/tutorials/tags/qwiic?page=all)
* [Select Sparkfun QWIIC Arduino libraries](https://github.com/search?q=topic%3Aarduino-library+org%3Asparkfun+qwiic)
* [Sparkfun QWIIC repositories at github](https://github.com/sparkfun?q=qwiic)

If you are unsure where to start, the
[Twist library](https://github.com/sparkfun/SparkFun_Qwiic_Twist_Arduino_Library)
has a number of
[examples](https://github.com/sparkfun/SparkFun_Qwiic_Twist_Arduino_Library/tree/master/examples).

If you would rather work on the SARA-R4 modem at the Arduino,
you may want to have a look at these documents:

* [Arduino MKR NB 1500 schematics](https://content.arduino.cc/assets/MKRNB1500_V3.0_sch.pdf)
* [SARA R4 datasheet](https://www.u-blox.com/sites/default/files/SARA-R4-N4_DataSheet_%28UBX-16024152%29.pdf)
* [SARA R4 AT Commands Manual](https://www.u-blox.com/sites/default/files/SARA-R4-SARA-N4_ATCommands_%28UBX-17003787%29.pdf)
* [SARA R4 Application Development Guide](https://www.u-blox.com/sites/default/files/SARA-R4-N4-Application-Development_AppNote_%28UBX-18019856%29.pdf)
* [SARA R4 other official documentation](https://www.u-blox.com/en/product-resources/property_file_product_filter/18030)
