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

PICrouter_OaUH (current ver. is 0.5)
--------------------------------
PICrouter_OaUH is for Open Sound Control and USB Host Function(MIDI only).  
PICrouter_OaUH.X is this project folder.

### Supported MIDI messages
1. Note On/Off (8nH/9nH)  
--> ***/midi/note [channel] [note_number] [velocity]***
2. Polyphonic Key Pressure (AnH)  
--> ***/midi/kp [channel] [note_number] [pressure]***
3. Control Change (BnH)  
--> ***/midi/cc [channel] [control_number] [control_value]***
4. Program Change (CnH)  
--> ***/midi/pc [program_number]***
5. Channel Pressure (DnH)  
--> ***/midi/cp [channel] [pressure]***
6. Pitch Bend (EnH)  
--> ***/midi/pb [channel] [pitch_bend_MSB] [pitch_bend_LSB]***

### System OSC Messages of Network Setting
1. ***/sys/remote/ip/set [xxx.xxx.xxx.xxx(string)]***
2. ***/sys/remote/ip/get [no arguments]***
3. ***/sys/host/name/set [host_name(string)]***
4. ***/sys/host/name/get [no arguments]***
5. ***/sys/host/ip/get [no arguments]***
6. ***/sys/host/mac/get [no arguments]***
7. ***/sys/prefix/set [prefix(string)]***
8. ***/sys/prefix/get [no arguments]***