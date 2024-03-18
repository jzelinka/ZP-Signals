## Description
The provided code manages to command light railway signals at model layout at [Železnice Podaba](https://www.zeleznicepodbaba.cz/en/). The hardware of individual stations is usally controlled using a ESP8266, which receives commands over MQTT protocol from a central server. The ESP8266 then controls the signals over I2C bus. The signals are grouped together on a PCB containing an Arduino Nano providing the connection to the I2C bus. The grouping of the signals allows for resource saving as not all signals need to posses their own chip and the resources are shared. Stations contain more PCBs with Arduino Nanos, as a single PCB would usually not suffice. The possible displable signs on the signal are of czech type, however it would be possible to modify the code to support other types of signals as well.
The code is constructed so that all the Arduino Nanos share the same code and only are distinguished by their I2C address. Such design allows for easy maintenance and updates of the code.

## Usage
Part of the code is written as a library for ArduinoIDE and thus has to be symlinked to the location of your ArduinoIDE libraries.
```bash
ln -s /path/to/this/repo/Signals /path/to/your/Arduino/libraries/Signals
```
This makes the library accessible anywhere in your ArduinoIDE projects.

Next you should compile and upload code from `arduino_rec` to your Arduino Nano. The TODO mark the place to change the I2C address of the Arduino Nano.

Finally you should compile and upload code from `exp_exec` to your ESP8266. More objects describing the signals can be added close to the TODO mark. Additionally a user has to create a `setup.h` file containing WIFI and MQTT credentials. A tepmlate file for the credentials is located in `exp_exec/setup_template.h`.

### Disclaimer
The code was developed for a czech model layout and thus some of the class, file or function names can contain czech words. In case of any questions feel free to contact me.