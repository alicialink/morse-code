TCP Morse Code
==========

This is project using Arduino to beep Morse Code as audio on a speaker, and optionally flash a light with the same Morse Code. It is connected to a network via a TCP service, which enables any device connected to the same network to Send Morse Code as audio.

Why did I create this? Personally, since I have known Morse Code since I was a kid, I find it useful to create (for lack of better terms) ambient audio signals from sensors and other devices from which I want to hear regular status updates. Because Morse Code is audio, not visual, I do not need to specifically direct my visual attention toward it. Morse Code is a very distinctive sound, and can be heard through other noises. By using this system, I can surround myself with an unobtrusive stream of information about interesting things.

## [akey7.com/morse-code is the main page for this project](http://akey7.com/morse-code "Main Page for Project") ##

There, you will find more documentation about parts required, assembly, and photographs of my current device.

On this GitHub page, I document `TcpMorseCode` (the sketch the empowers the operation of the embedded server) and `node-continuous-wave-client` (a trivial example in [Node.js](http://nodejs.org "Node.js homepage") to connect to the device and send morse code). Also on this page is a schematic of the circuit `schematic.png` that will work with the sketch.

Schematic
-----------

### See the file `schematic.png` for the circuit as connected to the Arduino ###

TcpMorseCode Sketch
-----------

This is the heart of the TCP serivce on the Arduino. It runs the server, translates the Morse Code into dots and dashes, and beeps the speaker and/or flashes the light

### Sketch Customization ###

There are few preprocessor directives, function calls, and variables to set up to configure the device to your network and morse code output. I have centralized these settings between the comments `/*** BEGIN software configuration ***/` and `/*** END software configuration ***/`

I have noted the most important configuration options here. There are more, but they are found and documented in the source code.

**First** Setup your Ethernet connectivity, depending on your netowrk. These are what works on my network. Yours will probably be different:

+ `IPAddress ip(192, 168, 0, 73);` The IP address for your device. Find a free IP address on your network.  *Note* This does not use DHCP. In the current version of the it just uses a static IP. Perhpas DHCP will come in the future.

+ `IPAddress gateway(192, 168, 0, 1);` The gateway on your network (though not strictly necessary)

+ `byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xC6, 0x1B };` The MAC address of your Ethernet interface. *This setting is crucial* every device on your Ethernet needs a different MAC address. My retail packaged [Arduino Ethernet Shield](http://arduino.cc/en/Main/ArduinoEthernetShield "Shield on Arduino site") came with a label on it that suggested a unique MAC address. See [MAC Address on Wikipedia](http://en.wikipedia.org/wiki/Mac_address "Wikipedia on MAC address") for some really dense technical infomration on MAC addresses.

**Second** Setup your Morse code speed

+ `#define CW_TICK_TIME_MILLISECONDS 56` The "tick" time in milliseconds (ms) that serves as the fundamental timing unit for Morse code generation. The default of 56 ms is a farily fast 19 words per minute (WPM) pace. Something around 220 ms is a better choice for beginner at about 6 WPM.

Operation
-----------

**Status LED** The RGB LED shows status of three colors:

+ *Red:* System is on, and setting up. The LED should only show red for a fraction of a second during setup.

+ *Blue:* System ready, waiting for a network connection to send data to sound off as Morse Code. The LED will shine blue most of the time, as it waits for a connection over the network with Morse Code to send.

+ *Green:* Data received, sending it as morse code.

**Visual Indicator LED** If configured in the code, this light will flash as the speaker beeps.

### Testing with `telnet` ###

To test the device, use `telnet` The commands below are made for my device. You will need to adjust the IP address for the device you mad eon your network.

```
$ telnet 192.168.0.73 73
Trying 192.168.0.73...
Connected to 192.168.0.73.
Escape character is '^]'.
HELLO WORLD
^]
telnet> quit
Connection closed.
```

You will see the status LED cycle through the colors, hear the morse code, and see the visual indicator flashing the morse code for "HELLO WORLD"

*Note* There are no letter cases in Morse code, so upper an lower case letters are the same.

Node.js Client
-----------

The following code snippet is in `cw-client.js`:

```
var net = require('net');

exports.send = function(message) {
  var client = net.connect({host: '192.168.0.73', port: 73}, function() {
    client.write(message);
    client.end();
  });
};
```

It simply connects to the device over the TCP port. You will need to modify the `host: '192.168.0.73'` to your IP address.

### Testing Node.js ###

In the `node-morse-code-client` directory, there is the `test-morse-code-client.js` script. run it like this:

```
node test-morse-code-client.js
```

As configured, it will send "I LOVE MORSE CODE" or whatever message you modify it to be.

Licenses
-----------

### Arduino Sketch ###

Copyright (c) 2013 Alicia M. F. Key

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

### Schematic Diagram Image ###

[Creative Commons Attribution-ShareAlike 3.0 Unported](http://creativecommons.org/licenses/by-sa/3.0/ "Creative commons license page")
