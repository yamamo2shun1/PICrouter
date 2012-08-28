PICrouter
================================
### Descriptions
PICrouter is a versatile board to develop musical instruments and controllers.  
And this is **Free Hardware** and **Free Software**.

![PICrouter](http://farm6.staticflickr.com/5076/7425609390_5f318ec3ee.jpg "PICrouter")

### Licenses
* Hardware : [Creative Commons Attribute-ShareAlike 3.0 Unported][cc-by-sa]  
* Software : GNU General Public License version 3  

[cc-by-sa]:http://creativecommons.org/licenses/by-sa/3.0/ "Creative Commons Attribute-ShareAlike 3.0 Unported"

### Specifications
1. Supported Open Sound Control
2. Supported USB device(MIDI and HID composite) and host(only MIDI) function.
3. Supported USB HID bootloader on Mac OS X and Windows.
4. PIC32MX675F512H(Freq:80MHz / FLASH:512K / RAM:64K)
5. A/D port x14
6. PWM output x4
7. SPI x2, I2C x3, UART x1, JTAG x1 and CAN x1
8. 100mm x 33.5mm, 4-layers board

### Development
* Microchip MPLABX v1.30
* Microchip XC32 Compiler v1.10

PICrouter Schematic and board (current ver. is 0.95)
--------------------------------
PICrouter.sch and .brd are drawn by Eagel v6.  
Additionaly, we use [Sparkfun eagle library][sparkfun].  
This library is also licensed by CC by-SA 3.0.  

And then We extend a special thank you to Tomoya.  
He gave many advices for designing of this board.

[sparkfun]: http://www.opencircuits.com/SFE_Footprint_Library_Eagle

PICrouter_HIDBL
--------------------------------
PICrouter_HIDBL.X is the USB HID Bootloader project files.  
Look at [AN1388][] for details.

[AN1388]: http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1824&appnote=en554836 "AN1388"

PICrouter_OaUD (current ver. is 0.2)
--------------------------------
PICrouter_OaUD is for Open Sound Control and USB Device Function(MIDI and HID).  
PICrouter_OaUD.X is this project folder.

PICrouter_OaUH (current ver. is 0.62)
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

### Standard OSC Messages of I/O
1. ***/std/pwm/state/set [state(string)]***
2. ***/std/pwm/state/get [no arguments]***
3. ***/std/pwm/freq/set [frequencey(int)]***
4. ***/std/pwm/freq/get [no arguments]***
5. ***/std/pwm/duty/set [duty(int)]***
6. ***/std/pwm/duty/get [no arguments]***

### System OSC Messages of Network Setting
1. ***/sys/remote/ip/set [xxx.xxx.xxx.xxx(string)]***
2. ***/sys/remote/ip/get [no arguments]***
3. ***/sys/remote/port/set [port_number(int)]***
4. ***/sys/remote/port/get [no arguments]***
5. ***/sys/host/name/set [host_name(string)]***
6. ***/sys/host/name/get [no arguments]***
7. ***/sys/host/ip/get [no arguments]***
8. ***/sys/host/mac/get [no arguments]***
9. ***/sys/host/port/set [port_number(int)]***
10. ***/sys/host/port/get [no arguments]***
11. ***/sys/prefix/set [prefix(string)]***
12. ***/sys/prefix/get [no arguments]***