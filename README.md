# HIDLCD Driver library for HID-compliant LCD Display 

This library allows host to communicate with [HID-compliant Auxiliary Display based on Arduino](https://github.com/abratchik/HIDAuxiliaryDisplay). 
The main idea of this library is to provide a simple unified communication with the small LCD displays 
supported by Arduino. Such displays can be used for showing system information such as hardware 
temperature, fan speed, etc., and used by host applications as necessary.

HIDLCD is a multi-platform library which allows an application to interface
with USB and Bluetooth HID-Class devices on Windows, Linux, FreeBSD, and macOS. It is build on top of the 
HIDAPI library and provides the next level of abstraction specific to the auxiliary LCD displays.

## Tested on Operating Systems
* Linux 18.04 LTS

## License 

Copyright (c) Alex Bratchik 2022. All right reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA