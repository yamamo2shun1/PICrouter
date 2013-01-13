PICrouter
================================
PICrouter is a versatile and prototyping board specialized for [Open Sound Control].  
The main function is to communicate between OSC and USB-MIDI and USB-HID.

[Open Sound Control]:http://opensoundcontrol.org

<div align="center">
<a href="http://www.flickr.com/photos/yamamo2/8369607593/" title="PICrouter v1.0 TOP VIEW by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8214/8369607593_3c1b5055e1_z.jpg" width="640" height="206" alt="PICrouter v1.0 TOP VIEW"></a>  
<a href="http://www.flickr.com/photos/yamamo2/8369762163/" title="PICrouter v1.0 ETH/USB VIEW by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8219/8369762163_daa2bac802_z.jpg" width="640" height="346" alt="PICrouter v1.0 ETH/USB VIEW"></a>  
</div>
<div align="right">
Photographed by Kiyasu/TechnikLAB*
</div>

Licenses
------
PICrouter is **Free Hardware** and **Free Software**.
* Hardware : [Creative Commons Attribute-ShareAlike 3.0 Unported][cc-by-sa]  
* Software : GNU General Public License version 3  

[cc-by-sa]:http://creativecommons.org/licenses/by-sa/3.0/ "Creative Commons Attribute-ShareAlike 3.0 Unported"

Specifications
------
0. Supported Zeroconf(Bonjour on Mac OS X), DHCP and NBNS
1. Supported Open Sound Control
2. Supported USB device(MIDI and HID composite) and host(only MIDI) function.
3. Supported [USB HID bootloader][an1388] on Mac OS X and Windows.
4. PIC32MX675F512H(Freq:80MHz / FLASH:512K / RAM:64K)
5. A/D port x14
6. PWM output x4
7. SPI x2, I2C x3, UART x1, JTAG x1 and CAN x1
8. 100mm x 33.5mm, 4-layers board

[an1388]:http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1824&appnote=en554836

Pin Assign
------
<div align="center">
<a href="http://www.flickr.com/photos/yamamo2/8371181045/" title="PICrouter v1.0 PIN ASSIGN by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8499/8371181045_7a317dd2ee_z.jpg" width="464" height="640" alt="PICrouter v1.0 PIN ASSIGN"></a>
</div>

Development
------
* Microchip MPLABX v1.60
* Microchip XC32 Compiler v1.20

Bootloader Application
------
* PIC32UBL.exe (Win / Download is [here][an1388])
* PIC32UBL_Max.app (Mac / See the folder "PICrouter/PIC32UBL_Max")

Schematic and Board (current ver. is 1.00)
--------------------------------
PICrouter.sch and .brd are drawn by Eagel v6.  
Additionaly, we use [Sparkfun eagle library][sparkfun].  
This library is also licensed by CC by-SA 3.0.  

And then We extend a special thank you to Tomoya(@tomoya723) and Rundog(@rundog).  
Tomoya gave many advices for designing of this board.  
Rundog assisted us with this board's production.


[sparkfun]: http://www.opencircuits.com/SFE_Footprint_Library_Eagle

Application Projects
------
#### PICrouter_HIDBL
PICrouter_HIDBL.X is the USB HID Bootloader project files.  
Look at [AN1388][] for details.

[AN1388]: http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1824&appnote=en554836 "AN1388"

#### PICrouter_OaUD (current ver. is 0.2)
PICrouter_OaUD is for Open Sound Control and USB Device Function(MIDI and HID).  
PICrouter_OaUD.X is this project folder.

#### PICrouter_OaUH (current ver. is 0.90)
PICrouter_OaUH is for Open Sound Control and USB Host Function(MIDI only).  
PICrouter_OaUH.X is this project folder.

Supported Open Sound Control Messages
--------------------------------
See [osc pages][].

[osc pages]: https://github.com/tkrworks/PICrouter/wiki/Supported-OSC-Messages
