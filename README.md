<div align="center">
<table>
<tr>
<th colspan="4" align="center">INDEX</th>
</tr>
<tr align="center">
    <td><a href="#how-to-buy">How to buy</a></td>
    <td><a href="#licenses">Licenses</a></td>
    <td><a href="#specifications">Specifications</a></td>
    <td><a href="#pin-assign">Pin Assign</a></td>
</tr>
<tr align="center">
    <td><a href="#development">Development</a></td>
    <td><a href="#bootloader-application">Bootloader App.</a></td>
    <td><a href="#schematic-and-board-current-ver-is-100">Sch & Brd Data</a></td>
    <td><a href="#application-projects">App. Projects</a></td>
</tr>
<tr align="center">
    <td><a href="#tutorial-jp">Tutorial (JP)</a></td>
    <td><a href="#tutorial-en">Tutorial (EN)</a></td>
    <td><a href="#supported-osc-messages">Supported OSC Msgs</a></td>
    <td><a href="#supported-hid-commands">Supported HID Cmds</a></td>
</tr>
</table>
</div>

PICrouter
======
PICrouter is a versatile and prototyping board specialized for [Open Sound Control].  
The main function is to communicate between OSC and USB-MIDI/HID.

PICrouter will be an useful with the following purpose.

* Develop an OSC/MIDI/HID controller for music or visual.
* Study a PIC32 microcontroller.
* Study a communication with usb and ethernet.

[Open Sound Control]:http://opensoundcontrol.org

<div align="center">
<a href="http://www.flickr.com/photos/yamamo2/8369607593/" title="PICrouter v1.0 TOP VIEW by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8214/8369607593_3c1b5055e1_z.jpg" width="640" height="206" alt="PICrouter v1.0 TOP VIEW"></a>  
<a href="http://www.flickr.com/photos/yamamo2/8369762163/" title="PICrouter v1.0 ETH/USB VIEW by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8219/8369762163_daa2bac802_z.jpg" width="640" height="346" alt="PICrouter v1.0 ETH/USB VIEW"></a>  
</div>
<div align="right">
Photographed by Kiyasu/TechnikLAB*
</div>

How to buy
------
We sell the PICrouter on tkrworks <a href="http://atelier.tkrworks.net/shop/index.php?main_page=product_info&cPath=2&products_id=29&language=en">online store</a>.  
The price of product is 7350JPY.

Licenses
------
PICrouter is **Free Hardware** and **Free Software**.
* Hardware : [Creative Commons Attribute-ShareAlike 3.0 Unported][cc-by-sa]  
* Software : GNU General Public License version 3  

[cc-by-sa]:http://creativecommons.org/licenses/by-sa/3.0/ "Creative Commons Attribute-ShareAlike 3.0 Unported"

Specifications
------
0. Supported mDNS(Bonjour on Mac OS X), DHCP(both server and client) and NBNS.
1. Supported Open Sound Control.
2. Supported USB dual role function(DEVICE:MIDI & HID composite / HOST:MIDI).
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
	<a href="http://www.flickr.com/photos/yamamo2/8371181045/" title="PICrouter v1.0 PIN ASSIGN by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8499/8371181045_ebda98c326_z.jpg" width="465" height="640" alt="PICrouter v1.0 PIN ASSIGN"></a>
</div>

Development
------
* Microchip [MPLABX] v1.70
* Microchip [MPLAB XC32 Compiler] v1.20  
  *To develop application, you can use free version.*

[MPLABX]: http://www.microchip.com/pagehandler/en-us/family/mplabx/
[MPLAB XC32 Compiler]: http://www.microchip.com/pagehandler/en_us/devtools/mplabxc/

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
#### PICrouter_HIDBL (current ver. is v1.1)  
  PICrouter_HIDBL.X is the USB HID Bootloader project files.  
  Look at [AN1388][] for details.  

[AN1388]: http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1824&appnote=en554836 "AN1388"

#### PICrouter (current ver. is 1.4.3) Updated 2013/03/26  
  PICrouter is for Open Sound Control and USB Dual Role.  
  PICrouter.X is this project folder.  
  The Max6 demo patches are in Max_Demo_Patchexs folder.  
  
#### PICrouter_OaUH (current ver. is 1.02)  
  PICrouter_OaUH is for Open Sound Control and USB Host Function(MIDI only).  
  PICrouter_OaUH.X is this project folder.  
  The Max6 demo patch is Max_Demo_Patchexs/OSC_USB(Host)_Demo/PICrouter_PICrouter-OaUH_Demo.maxpat.   
  * This project is test version for the HID host function. In the future,
    this will be merged into PICrouter.X.  

Tutorial (JP)
--------------------------------
Tutorial 0 : [まずはじめに][]  
Tutorial 1 : [OSCを使ったデジタル入出力の制御][]  
Tutorial 2 : [OSCを使ったアナログ入力の制御][]  
Tutorial 3 : [OSCを使ったPWMの制御][]  
Tutorial 4 : [OSCを使ったSPIの制御][]  
Tutorial 5 : [システム設定の変更]

[まずはじめに]: https://github.com/tkrworks/PICrouter/wiki/まずはじめに
[OSCを使ったデジタル入出力の制御]: https://github.com/tkrworks/PICrouter/wiki/OSCを使ったデジタル入出力の制御
[OSCを使ったアナログ入力の制御]: https://github.com/tkrworks/PICrouter/wiki/OSCを使ったアナログ入力の制御
[OSCを使ったPWMの制御]: https://github.com/tkrworks/PICrouter/wiki/OSCを使ったPWM出力の制御
[OSCを使ったSPIの制御]: https://github.com/tkrworks/PICrouter/wiki/OSCを使ったSPIの制御

Tutorial (EN)
--------------------------------
Tutorial 0 : [Quick Start Guide]  
Tutorial 1 : [How to Control Digital I/O with OSC]  
Tutorial 2 : [How to Control Analog Inputs with OSC][]  
Tutorial 3 : [How to Control PWM with OSC]  
Tutorial 4 : [How to Control SPI with OSC]  
[How to Control Analog Inputs with OSC]: https://github.com/tkrworks/PICrouter/wiki/How-to-Control-Analog-Inputs-with-OSC

Supported OSC Messages
--------------------------------
See [osc pages][].

[osc pages]: https://github.com/tkrworks/PICrouter/wiki/Supported-OSC-Messages

Supported HID Commands
--------------------------------
See [hid pages][].

[hid pages]: https://github.com/tkrworks/PICrouter/wiki/Supported-HID-Commands