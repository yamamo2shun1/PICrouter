PICrouter
================================
### Descriptions
PICrouter is a versatile board to develop musical instruments and controllers.  
And this is **Free Hardware** and **Free Software**.

![PICrouter](http://farm6.staticflickr.com/5076/7425609390_5f318ec3ee.jpg "PICrouter")

### Licenses
* Hardware : Creative Commons Attribute-ShareAlike 3.0 Unported  
* Software : GNU General Public License version 3  

### Specifications
1. Supported Open Sound Control
2. Supported USB device(MIDI and HID composite) and host(only MIDI) function.
3. Supported USB HID bootloader on Mac OS X and Windows.
4. PIC32MX675F512H(Freq:80MHz / FLASH:512K / RAM:64K)
5. A/D port x14
6. PWM output x4
7. SPI x2, I2C x3, UART x1, JTAG x1 and CAN x1
8. 100mm x 33.5mm, 4-layers board

PICrouter_HIDBL
--------------------------------
PICrouter_HIDBL.X is the USB HID Bootloader project files.  
Look at [AN1388][] for details.

[AN1388]: http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1824&appnote=en554836 "AN1388"

PICrouter_OaUD (current ver. is 0.2)
--------------------------------
PICrouter_OaUD is for Open Sound Control and USB Device Function(MIDI and HID).  
PICrouter_OaUD.X is this project folder.

PICrouter_OaUH (current ver. is 0.1)
--------------------------------
PICrouter_OaUH is for Open Sound Control and USB Host Function(MIDI only).  
PICrouter_OaUH.X is this project folder.