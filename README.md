# ArduECO

ArduECO is an IoT device developed using a NodeMCU board programmed with Arduino's C/C++ capable of tracking the amount of *CO (Carbon Monoxide)* in the air with a MQ-7 sensor, along GPS position and send it in the cloud to AWS's *IoT Core*. These are stored in a simple SQL database and displayed on a website using Google Maps' APIs.

This project has been created for the 

[Wireless Networks]: https://www.math.unipd.it/~cpalazzi/retiwireless.html	"Wireless Networks"

 course held by 

[Prof. Claudio Palazzi]: https://www.math.unipd.it/~cpalazzi/	"Prof. Claudio Palazzi"

, in the 

[Master's in Computer Science]: http://informatica.math.unipd.it/laureamagistrale/indexen.html	"Master's in Computer Science"

 at Università degli Studi di Padova.

The goal of this project was to develop a PoC of an easy way to keep track in real time of air quality using devices that can be easily installed on shared green methods of transportation such as bikes and electric scooters from companies like 

[Mobike]: https://mobike.com/global/	"Mobike"

.

Further information about the project can be found in the document essay.

## Schematics
<img src="K:\UNI\ArduECO\essay\fig1.png" style="zoom:50%;" />



The circuit have been designed using 

[Fritzing]: https://fritzing.org/	"Fritzing"

.

## Software

### Arduino
#### Libraries
In order to complete this project I have used some additional libraries
### AWS
#### IoT Core
#### Lambda
### Website
#### PHP
#### Database
#### End result
## Improvements

Better algorithm for uploading data
Better hardware
Multiple air sensors
