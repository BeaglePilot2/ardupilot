#APMsubmarine v0.1

To compile the vehicle code on a linux distro -

    git clone https://github.com/BeaglePilot2/ardupilot.git

    cd ardupilot/APMsubmarine/

    make linux

Alternatively, you can also cross-compile the vehicle code for the Erle-Brain -

    git clone https://github.com/BeaglePilot2/ardupilot.git

    sudo apt-get install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf

    cd ardupilot/APMsubmarine/

    make pxf
