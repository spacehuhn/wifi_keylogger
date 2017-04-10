# wifi_keylogger
(Proof of Concept) Arduino Keylogger with Wi-Fi! 

![arduino leonardo with usb host shield and a nodemcu](https://raw.githubusercontent.com/spacehuhn/wifi_keylogger/master/images/keylogger_with_nodemcu_2.jpg)

## Contents
- [Introduction](#introduction)
- [Disclaimer](#disclaimer)
- [Installation](#installation)
  - [Preparation](#preparation)  
  - [ESP8266](#esp8266)
  - [Arduino ATmega32u4](#arduino-atmega32u4)
  - [Wire everything up](#wire-everything-up)
- [How to use it](#how-to-use-it)
- [License](#license)
- [Sources and additional Links](#sources-and-additional-links)

## Introduction ##

Using an Arduino with an ATmega32u4 (which can emulate a keyboard) and a USB host shield, this combination can be used as a keylogger. The addition of an ESP8266 allows you to store and retrieve the keystrokes in a log file.  

**This is just a proof of concept**
Please don't expect this to work with every keyboard layout!  
I saw a few forum threads about making a USB keylogger with Arduino using a USB Host shield, but I couldn't find any published project about this.  
So I hope this is a good basis for someone who wants to make a proper keylogger, though **I won't develop this any further**.  

## Disclaimer

Use it only for testing purposes on your own devices!  
I don't take any responsibility for what you do with this project.  

## Installation

### Short version:
Upload the `keylogger` sketch to your Arduino (ATmega32u4) and upload the `esp8266_saveSerial` sketch to your ESP8266.  
Connect your Arduino with the USB host shield and connect the serial pins (RX and TX (Arduino) to TX and RX (ESP8266)) and GND.

---

### Preparation

What you will need:
- **ESP8266 Wi-Fi chip**  
- **Arduino with an ATmega32u4**  
- **USB Host Shield**
  There are different versions out there  
![usb host shields](https://raw.githubusercontent.com/spacehuhn/wifi_keylogger/master/images/usb_host_shields.jpg)  
Have a look at the official site: https://www.circuitsathome.com/usb-host-shield-hardware-manual/

I used an Arduino Leonardo with the big host shield.  

![arduino leonardo with USB host shield and a nodemcu](https://raw.githubusercontent.com/spacehuhn/wifi_keylogger/master/images/keylogger_with_nodemcu.jpg)  

You could probably build a very small version using the mini host shield and a pro micro.  
http://forum.arduino.cc/index.php?topic=325930.0


### ESP8266

Open the `esp8266_saveSerial` sketch with [Arduino](https://www.arduino.cc/en/Main/Software).  
You need to install the following Librarys:
- [the latest ESP8266 SDK](https://github.com/esp8266/Arduino)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)

Then compile and upload it to your ESP8266 (check if your settings are right).  

### Arduino ATmega32u4

You will need the [USB Host Shield 2.0 Library](https://github.com/felis/USB_Host_Shield_2.0).  
Open the `keylogger` sketch in Arduino and upload it to your Arduino.  

### Wire everything up

Ok so now you need to connect the ESP8266 with the Arduino.  
Connect these pins:  

| Arduino       | ESP82666      |
| ------------- |:-------------:|
| TX            | RX            |
| RX            | TX            |
| GND           | GND           |
| VCC (3.3V)    | VCC (3.3V)    |

**Note:** you'll need a 3.3V regulator if your Arduino only provides 5V.  
**Don't connect the ESP8266 to 5V!**  

If you use a plain ESP-12, you also have to set the enable pin and to HIGH and GPIO15 to LOW:  

| PIN          | Mode       |
| ------------ |:----------:|
| GPIO15       | LOW (GND)  |
| CH_PD (EN)   | HIGH (3.3V)| 

## How to use it

Plug the leonardo in the computer and the keyboard into the USB host shield. The ESP8266 will create a new access point `definitely not a keylogger`. Connect to it using the password `!keylogger`.  
Open your browser and go to `192.168.4.1`, you will see every keytroke.  

![screenshot of the webinterface](https://raw.githubusercontent.com/spacehuhn/wifi_keylogger/master/images/screenshot.JPG)

To clear the logfile go to `192.168.4.1/clear`.  

## License

This project is licensed under the MIT License - see the [license file](LICENSE) file for details

## Sources and additional Links

USB Host shield:  https://www.circuitsathome.com/usb-host-shield-hardware-manual/
                  https://www.arduino.cc/en/Main/ArduinoUSBHostShield
