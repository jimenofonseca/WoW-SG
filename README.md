# WoW-SG
![GitHub license](https://img.shields.io/github/license/jimenofonseca/wow-sg) ![Repo Size](https://img.shields.io/github/repo-size/jimenofonseca/wow-sg) ![Lines](https://github.com/jimenofonseca/wow-sg/blob/image-data/badge-lines-of-code.svg)

Weather station on wheels for beginners
 
 ## How does it work

 This is the code for the control and real-time data visualization of a mini weather-station mounted on a small plastic robot car. 
 It is a great starting point for learning about two way comunications with micro-controllers (in C++) and on how to create a web-based dashboard (in python).
 The car can be operated with the Joystick in the dashboard or with the line-tracking sensor onboard (see more information about the avialable operation modes in the next graph).
 
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
  - motor driver library (depending on what unit you buy)
  - DHT sensor library

## FAQ

- How much does this project cost? A: about 30 USD + 8 hours of your time (if you want to go into the details of course).
- Where is the code for controlling the unit? A: in the folder "control". You can either upload the arduino code for Remote Control mode or Line-tracking mode.
- How do I initialize the web-based dashboard? A: Make sure you connect your computer to the Bluetooth unit. Then run the script "interface/main.py". the dashboard should open up in a new window of your default internet browser
