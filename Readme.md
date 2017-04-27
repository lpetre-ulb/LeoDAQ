# Base VME DAQ
## Requirements:
1. Ubuntu 14.04 or more recent
2. qt5 libraries
<!--
  * you can get this simply by the command sudo apt-get install qt5-default.
  * type qmake -v in order to check that you have the qt5 version
-->
  * wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run
  * chmod +x qt-opensource-linux-x64-5.7.0.run ./qt-opensource-linux-x64-5.7.0.run 
  * sudo apt-get install build-essential

## Installation:
1. Compile and install the CAEN USB Driver
  1. Go to CAENUSBdrvB-1.5.1
  2. make
  3. sudo make install
2. Compile and install the CAEN VME C libraries
  1. Go to CAENVMELib-2.50/lib
  2. sudo ./install_x64 
3. Compile the Base VME DAQ software
  1. qmake BaseVMEDAQ.pro
  2. make
  3. sudo make install (this step is optional)
4. Execute the programme
  1. ./bin/BaseVMEDAQ or from a double-click on the desktop icon if point 3.3. was done

