# DataGrid

A monitoring system for capturing environmental properties in real-time. 

The main motivation for this development was to establish a system that measures and records a large amount of invisible matter (chemicals, particles, frequencies), in contrast to other commercial solutions where their scope is often limited, and to develop an open-source framework based on accessible and affordable sensors and electronics. 

The system uses an Arduino Mega. Sensor devices are connected to the micro-controller, where their readings are digitized and shared on the server database using an Ethernet connection. The sensors are able to scan temperature, humidity, light intensity, sound intensity, liquefied petroleum gas (LPG), butane, propane, methane, hydrogen, nitrogen, smoke, ozone, carbon monoxide, carbon dioxide, particulate matter, and radiation. Moreover, the device includes a Light Emitting Diode (LED) screen that displays the sensor readings and a piezoelectric speaker, which alerts users if a particular value exceeds the predefined limits so that caution can be taken.

<p align="center">
<img alt="DataGrid" src="assets/diagram.png" width="480" />
</p>
