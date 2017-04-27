# LeoDAQ
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
   * Go to CAENUSBdrvB-1.5.1
   * make
   * sudo make install
2. Compile and install the CAEN VME C libraries
   * Go to CAENVMELib-2.50/lib
   * sudo ./install_x64 
3. Compile the LeoDAQ software
   * qmake LeoDAQ.pro
   * make
   * sudo make install
4. Execute the programme
   * ./bin/LeoDAQ #or from a double-click on the desktop icon if a make install was done
5. Make it available for other users (optional)
   * sudo cp ~/Desktop/LeoDAQ.desktop otherUser/Desktop/.
   * sudo chown otherUser Desktop/LeoDAQ.desktop
  
## Update:
1. git pull
2. qmake
3. make -j8
4. sudo make install
