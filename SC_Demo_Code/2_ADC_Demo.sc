// PICrouter demo code for SuperCollider
// Created by Koichiro Mori 2013.
// This content is published under CC-BY-SA 3.0 license.


thisProcess.openUDPPort(8000);// At first you should open a UDP port..

n = NetAddr("picrouter.local",8080); // address

// Control ADC

n.sendMsg("/std/adc/enable/set", 0, "on"); // set adc enable of pin 0

(
OSCFunc(
	{ |msg, time, addr, recvPort|
		msg.postln;
	},
	'/std/adc/enable',
	nil
);
)

n.sendMsg("/std/adc/enable/get", 0); // get adc stateus of pin 0

n.sendMsg("/std/adc/enable/set", 0, "off"); // set adc disable of pin 0


// get ADC value
(
OSCFunc(
	{ |msg, time, addr, recvPort|
		msg.postln;},
	'/std/adc',
	nil
);
)

n.sendMsg("/std/adc/enable/set", 0, "on"); // on and start sending

n.sendMsg("/std/adc/enable/set", 0, "off"); // off and stop sending
