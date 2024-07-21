#!/bin/bash

CMAKE_install=2
DEFALUT_CMAKE='yes'

# Conditional check for installation
if [ "$DEFALUT_CMAKE" == "yes" ]; then
    sudo apt remove cmake
    sudo apt purge cmake
    sudo apt remove --purge cmake
    sudo rm -rf /usr/local/bin/cmake
    sudo rm -rf /usr/local/share/cmake*
    sudo rm -rf /usr/local/doc/cmake*
    sudo rm -rf /usr/local/man/man1/cmake*
    sudo rm -rf /usr/local/man/man7/cmake*
    sudo apt update
    sudo apt install cmake
fi

# Function to install CMake 3.15.7
install_cmake_3_15_7() {
    # Install required build tools
    sudo apt-get update
    sudo apt-get install -y build-essential

    # Download CMake 3.15.7 source code
    wget https://cmake.org/files/v3.15/cmake-3.15.7.tar.gz

    # Extract the archive
    tar -xzvf cmake-3.15.7.tar.gz

    # Navigate to the extracted directory
    cd cmake-3.15.7

    # Compile and install
    ./bootstrap --prefix=/home/ubuntu1604/cmake
    make
    sudo make install
    export PATH=/home/gokul/cmake/bin:$PATH
    #To make this change permanent, add the above line to your ~/.bashrc file:
    #echo 'export PATH=/home/gokul/cmake/bin:$PATH' >> ~/.bashrc
    #source ~/.bashrc
}

# Function to install CMake 3.15.0
install_cmake_3_15_0() {
    # Install required build tools
    sudo apt-get update
    sudo apt-get install -y build-essential

    # Remove existing CMake installation if present
    sudo apt-get remove --auto-remove --purge cmake

    # Download CMake 3.15.0 source code
    wget https://github.com/Kitware/CMake/releases/download/v3.15.0/cmake-3.15.0.tar.gz

    # Extract the archive
    tar -xzvf cmake-3.15.0.tar.gz

    # Navigate to the extracted directory
    cd cmake-3.15.0

    # Compile and install
    ./bootstrap --prefix=/home/ubuntu1604/cmake
    make
    sudo make install
    export PATH=/home/gokul/cmake/bin:$PATH
    #To make this change permanent, add the above line to your ~/.bashrc file:
    #echo 'export PATH=/home/gokul/cmake/bin:$PATH' >> ~/.bashrc
    #source ~/.bashrc
}

# Conditional check for installation
if [ "$CMAKE_install" = "1" ]; then
    install_cmake_3_15_7
elif [ "$CMAKE_install" = "2" ]; then
    install_cmake_3_15_0
else
    echo "Invalid CMAKE_install value. No action taken."
fi
