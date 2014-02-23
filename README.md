CSGames 2014 - Embedded programming competition
=====================================

Synopsis
-------------
Welcome to CS Games 2014 edition of the embedded programming competition. This year the competition wont be so much about embed centric programmation but more about integration with other system like a computer. 

To do so, protocols are designed and used to allow communication between systems. The problem is: the communication channels are not always reliable and some corruption can happen from time to time in an unpredictable manner.

The challenge
--------------------
This year challenge is to design and implement a communication protocol over an unreliable serial communication channel. An electronic input and acquisition board has been design. It shall communicate with a GUI interface on a remote control computer. The source code is almost finished. The only missing part is the communication protocol to toggle feature on the board from the computer and to send acquisition data from the board to the computer.

The serial communication channel is not to be trusted. Various kind of nasty corruption happen in a sporadic manner and needs to be taken into account. Bit flips, missing bytes, and wire disconnection and reconnection are some of few possible issues happening over the wire.

Material
--------------

- Atmel EVK1100 evaluation board featuring an AVR32 AT32UC3A0512 microcontroller
- A Serial cable to plug into the UART1 port
- An USB cable for programmation
- A C++ QT GUI application has been developed. The GUI is complete and functional. The byte reception parsing and sending needs to be done.

Test setup
---------------

The test setup to evaluate your submission to the competition will look something like this:

    (EVK1100 board) <---rs232---> (Magic test box(corruption)) <---rs232---> (Computer)

Information to exchange over serial
---------------------------------------------------

###Board input to report to the computer

- 3 momentary push buttons
- 1 temperature sensor
- 1 potentiometer

###Board output to control from the computer

- LED red intensity
- LED yellow intensity
- Variable length text to display on the screen

Quick start
---------------

###Flash procedure

To put the EVK1100 board in programming mode, __hold the joystick center button and press reset__

Then

    cd csgames_embed_avr32
    make flash

###IDE

An eclipse project has been setup for AVR32 development. Just import existing project in eclipse

A Qt project has been setup for linux Qt development with serial port support. Open QtCreator and open csgames_embed_qt the.pro project file.

Correction
----------------

TBD


