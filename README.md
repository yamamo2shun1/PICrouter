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
* Study a mruby(Support ONLY PIC32MX795F512H version)

Demo

* [Oto Devices Quick Demo][]
* [PICrouter and MobMuPlat OSC Demo][]
* [PICrouter and touchOSC Demo: PWM Control][]
* [LED Step Sequencer Test with PICrouter and LED_PAD_16][]
* [LED Intensity Control Test with PICrouter and LED_PAD_16][]
* [Multiple LED_ENC_ABS_32 Control Test from PICrouter][]
* [Intensity Control Test of RGB_PAD_16 from PICrouter][]
* [RGB Step Sequencer Test][]

[Open Sound Control]:http://opensoundcontrol.org

[Oto Devices Quick Demo]:https://vimeo.com/67548325
[PICrouter and MobMuPlat OSC Demo]:http://www.youtube.com/watch?v=xbNTXSsHR5w
[PICrouter and touchOSC Demo: PWM Control]:http://www.youtube.com/watch?v=srX1inH3xIY
[LED Step Sequencer Test with PICrouter and LED_PAD_16]: http://youtu.be/5rHGf42VQA8
[LED Intensity Control Test with PICrouter and LED_PAD_16]: http://youtu.be/yXPXuEcpFhE
[Multiple LED_ENC_ABS_32 Control Test from PICrouter]: http://youtu.be/imvcYX0qTLs
[Intensity Control Test of RGB_PAD_16 from PICrouter]: https://vimeo.com/70006217
[RGB Step Sequencer Test]: https://vimeo.com/73281314

<div align="center">
<a href="http://www.flickr.com/photos/yamamo2/8369607593/" title="PICrouter v1.0 TOP VIEW by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8214/8369607593_3c1b5055e1_z.jpg" width="640" height="206" alt="PICrouter v1.0 TOP VIEW"></a>  
<a href="http://www.flickr.com/photos/yamamo2/8369762163/" title="PICrouter v1.0 ETH/USB VIEW by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8219/8369762163_daa2bac802_z.jpg" width="640" height="346" alt="PICrouter v1.0 ETH/USB VIEW"></a>  
</div>
<div align="right">
Photographed by Kiyasu/TechnikLAB*
</div>

How to buy
------
You can buy the PICrouter on <a href="http://atelier.tkrworks.net/shop/index.php?main_page=product_info&cPath=2&products_id=29&language=en">tkrworks online store</a> and <a href="http://www.switch-science.com/catalog/1338/">SWITCH SCIENCE</a>.  
The price of product is 7350JPY.

Licenses
------
PICrouter is **Free Hardware** and **Free Software**.
* Hardware : [Creative Commons Attribute-ShareAlike 3.0 Unported][cc-by-sa]  
* Software : GNU General Public License version 3  

[cc-by-sa]:http://creativecommons.org/licenses/by-sa/3.0/ "Creative Commons Attribute-ShareAlike 3.0 Unported"

Specifications
------
0. Supported mruby(ONLY PIC32MX795F512H version)
1. Supported mDNS(Bonjour on Mac OS X), DHCP(both server and client).
2. Supported Open Sound Control.
3. Supported the multicast address(224.0.0.1 and 224.0.0.251).
4. Supported USB dual role function(DEVICE:MIDI & HID composite / HOST:MIDI & CDC composite).
5. Supported [USB HID bootloader][an1388] on Mac OS X and Windows.
6. PIC32MX675F512H(Freq:80MHz / FLASH:512K / RAM:64K) or PIC32MX795F512H(Freq:80MHz / FLASH:512K / RAM:128K)
7. A/D port x14
8. PWM output x4
9. SPI x2, I2C x3, UART x1, JTAG x1 and CAN x1
10. 100mm x 33.5mm, 4-layers board

[an1388]:http://www.microchip.com/stellent/idcplg?IdcService=SS_GET_PAGE&nodeId=1824&appnote=en554836

Pin Assign
------
<div align="center">
	<a href="http://www.flickr.com/photos/yamamo2/8371181045/" title="PICrouter v1.0 PIN ASSIGN by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8499/8371181045_ebda98c326_z.jpg" width="465" height="640" alt="PICrouter v1.0 PIN ASSIGN"></a>
  <a href="http://www.flickr.com/photos/yamamo2/8658610035/" title="PICrouter v1.0 PIN ASSIGN by tkrworks, on Flickr"><img src="http://farm9.staticflickr.com/8108/8658610035_d72ed7cd0a_z.jpg" width="486" height="640" alt="PICrouter v1.0 PIN ASSIGN"></a>
</div>

Development
------
* Microchip [MPLABX] v2.00
* Microchip [MPLAB XC32 Compiler] v1.22  
  *To develop application, you can use free version.*

[MPLABX]: http://www.microchip.com/pagehandler/en-us/family/mplabx/
[MPLAB XC32 Compiler]: http://www.microchip.com/pagehandler/en_us/devtools/mplabxc/

Bootloader Application
------
* PIC32UBL.exe (Win / Download is [here][an1388])
* PIC32UBL.maxpat (Mac / See the readme.txt in the folder "PICrouter/PIC32UBL_Max")

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

#### PICrouter (current ver. is 1.11.0) Updated 2013/11/18  
  PICrouter is for Open Sound Control and USB Dual Role.  
  PICrouter.X is this project folder.  
  The Max6 demo patches are in Max_Demo_Patchexs folder.  

#### PICrouter for mruby (current ver. is 0.2.3) Updated 2014/02/22  
  PICrouter for mruby supports mruby, Open Sound Control and USB Dual Role.  
  PICrouter_795_mrb.X is this project folder.
  
Tutorial (JP)
--------------------------------
Tutorial 0 : [まずはじめに][]  
Tutorial 1 : [OSCを使ったデジタル入出力の制御][]  
Tutorial 2 : [OSCを使ったアナログ入力の制御][]  
Tutorial 3 : [OSCを使ったPWMの制御][]  
Tutorial 4 : [OSCを使ったSPIの制御][]  
Tutorial 5 : [システム設定の変更][]  
Tutorial 6 : [MIDIをOSCに変換する][]  
Tutorial 7 : [ファームウェアの書き換え][]  
Tutorial 8 : [LED_ENC_32をPICrouterからOSCで制御][]  
Tutorial 9 : [LED_ENC_ABS_32をPICrouterからOSCで制御][]  
Tutorial 10 : [LED_PAD_16をPICrouterからOSCで制御][]

[まずはじめに]: https://github.com/tkrworks/PICrouter/wiki/まずはじめに
[OSCを使ったデジタル入出力の制御]: https://github.com/tkrworks/PICrouter/wiki/OSCを使ったデジタル入出力の制御
[OSCを使ったアナログ入力の制御]: https://github.com/tkrworks/PICrouter/wiki/OSCを使ったアナログ入力の制御
[OSCを使ったPWMの制御]: https://github.com/tkrworks/PICrouter/wiki/OSCを使ったPWM出力の制御
[OSCを使ったSPIの制御]: https://github.com/tkrworks/PICrouter/wiki/OSCを使ったSPIの制御
[システム設定の変更]: https://github.com/tkrworks/PICrouter/wiki/システム設定の変更
[MIDIをOSCに変換する]: https://github.com/tkrworks/PICrouter/wiki/MIDIをOSCに変換する
[ファームウェアの書き換え]: https://github.com/tkrworks/PICrouter/wiki/ファームウェアの書き換え
[LED_ENC_32をPICrouterからOSCで制御]: https://github.com/tkrworks/PICrouter/wiki/LED_ENC_32をPICrouterで制御
[LED_ENC_ABS_32をPICrouterからOSCで制御]: https://github.com/tkrworks/PICrouter/wiki/LED_ENC_ABS_32をPICrouterで制御
[LED_PAD_16をPICrouterからOSCで制御]: https://github.com/tkrworks/PICrouter/wiki/LED_PAD_16をPICrouterで制御

Tutorial (EN)
--------------------------------
Tutorial 0 : [Introduction][]  
Tutorial 1 : [Digital IO Control with OSC][]  
Tutorial 2 : [Analog Input Control with OSC][]  
Tutorial 3 : [PWM Output Control with OSC][]  
Tutorial 4 : [SPI Configuration with OSC][]  
Tutorial 5 : [System Configuration with OSC][]  
Tutorial 6 : [OSC-MIDI Bridge][]  
Tutorial 7 : [Firmware Update][]  
Tutorial 8 : [Control LED_ENC_32 from PICrouter with OSC][]  
Tutorial 9 : [Control LED_ENC_ABS_32 from PICrouter with OSC][]  
Tutorial 10 : [Control LED_PAD_16 from PICrouter with OSC][]

[Introduction]: https://github.com/tkrworks/PICrouter/wiki/Introduction
[Digital IO Control with OSC]: https://github.com/tkrworks/PICrouter/wiki/Digital-IO-Control-with-OSC
[Analog Input Control with OSC]: https://github.com/tkrworks/PICrouter/wiki/Analog-Input-Control-with-OSC
[PWM Output Control with OSC]: https://github.com/tkrworks/PICrouter/wiki/PWM-Output-Control-with-OSC
[SPI Configuration with OSC]: https://github.com/tkrworks/PICrouter/wiki/SPI-Configuration-with-OSC
[System Configuration with OSC]: https://github.com/tkrworks/PICrouter/wiki/System-Configuration-with-OSC
[OSC-MIDI Bridge]: https://github.com/tkrworks/PICrouter/wiki/OSC-MIDI-Bridge
[Firmware Update]: https://github.com/tkrworks/PICrouter/wiki/Firmware-Update
[Control LED_ENC_32 from PICrouter with OSC]: https://github.com/tkrworks/PICrouter/wiki/Control-LED_ENC_32-from-PICrouter
[Control LED_ENC_ABS_32 from PICrouter with OSC]: https://github.com/tkrworks/PICrouter/wiki/Control-LED_ENC_ABS_32-from-PICrouter
[Control LED_PAD_16 from PICrouter with OSC]: https://github.com/tkrworks/PICrouter/wiki/Control-LED_PAD_16-from-PICrouter-with-OSC

Supported OSC Messages
--------------------------------
See [osc pages][].

[osc pages]: https://github.com/tkrworks/PICrouter/wiki/Supported-OSC-Messages

Supported HID Commands
--------------------------------
See [hid pages][].

[hid pages]: https://github.com/tkrworks/PICrouter/wiki/Supported-HID-Commands
