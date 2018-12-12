# Occupancy Detection

An IOT device to track the number of people entering, leaving, and present within a house. The occupancy information can be sent to a central IoT server with Xbee modules wirelessly. 

## Apporach 1:

An ultrasonic module that uses ultrasonic sensors and an AVR controller.  The ultrasonic sensors use sound waves to determine the distance to an object from the device. The idea is was to use multiple sensors to monitor the doors and determine if people are entering or leaving, and through this determine the number of people present in the house, and where they are located. 


## Approach 2: 

An camera module that uses camera and a raspberry Pi. The idea was to use pedestrian tracking software, and some door recognition software to track the doors and people. In the end this information was used to determine where the people were moving, how many people entered rooms, and which rooms they entered.

## Hardware Requirement

List of Items for Approaches 1：

ATMega 2560

Ultrasonic Sensor - HC-SR04

Dual 4:1 MUX/DEMUX 

XBEE S2C DIGIMESH

Voltage Regulator

and battery

List of Items for Approaches 2：

Raspberry Pi 3 

Raspberry Pi Camera V2

5V 2.5A Micro USB power supply 

MicroSD Card

## Overall Design

![highlevel](https://user-images.githubusercontent.com/42009303/49890942-f0ed1f00-fe0a-11e8-96a2-07cd990ee364.png)


## Approach 1 Hardware Schematics 

![hardware](https://user-images.githubusercontent.com/42009303/49890937-f0548880-fe0a-11e8-9417-ef1461732b3c.png)
