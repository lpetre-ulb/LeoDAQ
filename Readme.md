# LeoDAQ
## Requirements:
1. Ubuntu 14.04 or more recent
2. Install qt5 libraries (version >= 5.7)
   * wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run
   * chmod +x qt-opensource-linux-x64-5.7.0.run ./qt-opensource-linux-x64-5.7.0.run
   * sudo apt-get install build-essential

## Installation:
1. Compile and install the CAEN USB Driver
   * Go to CAENUSBdrvB-1.5.1
   * make
   * sudo make install
2. Install the CAEN VME C libraries
   * Go to CAENVMELib-2.50/lib
   * sudo ./install_x64
3. Compile the LeoDAQ software
   * qmake LeoDAQ.pro
   * make -j8
   * sudo make install
4. Execute the programme
   * ./bin/LeoDAQ #or from a double-click on the desktop icon if a make install was done
5. Make it available for other users (optional)
   * sudo cp ~/Desktop/LeoDAQ.desktop otherUser/Desktop/.
   * sudo chown otherUser Desktop/LeoDAQ.desktop
  
## Update:
1. git pull
2. qmake LeoDAQ.pro
3. make -j8
4. sudo make install

