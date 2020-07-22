# WoW-SG
![GitHub license](https://img.shields.io/github/license/jimenofonseca/wow-sg) ![Repo Size](https://img.shields.io/github/repo-size/jimenofonseca/wow-sg) ![Lines](https://github.com/jimenofonseca/wow-sg/blob/image-data/badge-lines-of-code.svg)

Weather station on wheels for beginners
 
 ## How does it work

 This is the code for the control and real-time data visualization of a mini weather-station mounted on a small plastic robot car. 
 It is a great starting point for learning about two way comunications with micro-controllers (in C++) and on how to create a web-based dashboard (in python).
 The car can be operated with the Joystick in the dashboard or with the line-tracking sensor onboard (see more information about the available operation modes in the next figure).
 
 ![summary](https://github.com/jimenofonseca/wow-sg/blob/master/images/summary.PNG)
 

## Installation

Assuming that you have Python >3.6 and the latest Arduino console installed:

- Clone this repository
- Install python dependencies in setup.py

  - 'serial'
  - 'matplotlib'
  - 'dash'
  - 'dash-daq'
  - 'dash-core-components'
  - 'dash-html-components'
  - 'plotly'
  - 'pybluez'
  
- Install the next Arduino dependencies (libraries):

  - ArduinoJson
  - Motor driver library (depending on what unit you buy)
  - DHT sensor library

## FAQ

- How much does this project cost? A: about 30 USD + X hours of your time (if you want to go into the details ofcourse).
- Where do I get the parts? A: any big online store has them (most of them will be in China). Do not forget to get at least one small breadboard and aobut 15 male-male jumper cables!
- Where is the code for controlling the unit? A: in the folder "control_mode". You can either upload the arduino code for Remote Control mode or Line-tracking mode.
- How do I initialize the web-based dashboard? A: Make sure you connect your computer to the Bluetooth unit. Then run the script "interface/main.py". the dashboard should open up in a new window of your default internet browser
- What would you change? A: the bluetooth device / connection is not very stable, I would use another protocol or an independent receiver and transmitter unit if possible. I would also use MQTT to better manage the channel of information back from the mini weather station to the dashboard.
