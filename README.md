# HIDLCD Driver library for HID-compliant LCD Display 

This driver allows the host to communicate with [HID-compliant Auxiliary Display based on Arduino](https://github.com/abratchik/HIDAuxiliaryDisplay). 
The main idea of this library is to provide a simple unified communication with the small LCD displays 
supported by Arduino. Such displays can be used for showing system information such as hardware temperature, 
fan speed, percentage of memory available etc., and used by host applications as necessary.

HIDLCD is build on top of the HIDAPI library and provides the next level of abstraction specific to the HID-compliant 
auxiliary LCD displays.

## Supported displays
Compatible types of displays include:
* 1602 LCD Display 
* 1604 LCD Display (to be tested) 
* 2004 LCD Display (to be tested)

## Supported operating systems
* Linux (tested on Ubuntu 18.04 LTS)

## Architecture of the solution
The overall architecture of the solution is represented on the image below:

![Alt text](https://github.com/abratchik/HIDAuxiliaryDisplay/img/HIDAuxiliaryDisplayArchitecture.png)

The solution is consisted of 2 parts - the HID-compliant LCD display and the host. The host part 
contains the auxdisplay service, the HIDLCD driver (this project) and the HIDAPI library.

## Installation
Currently, only Linux hosts are supported so the installation procedure below describes the steps to be performed 
on Linux.

Before executing the steps below, please ensure the developer tools are installed on your host. If your operating 
system is Ubuntu, you can run `sudo apt update` & `sudo apt install build-essential` in the bash terminal. 

1. Install [HIDAPI library](https://github.com/libusb/hidapi). Just follow the instructions in HIDAPI Readme.
2. Clone this repository to a folder on your host machine.
3. Navigate to the folder and execute `make` and `sudo make install`

## Usage
TBD

## License

[GPL v.3](http://www.gnu.org/copyleft/gpl.html)