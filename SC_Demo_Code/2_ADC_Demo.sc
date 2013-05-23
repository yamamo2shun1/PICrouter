// PICrouter demo code for SuperCollider
// Created by Koichiro Mori 2013.
// This content is published under CC-BY-SA 3.0 license.


thisProcess.openUDPPort(8000);// At first you should open a UDP port..

// Controll ADC
NetAddr("picrouter.local",8080).sendMsg("/std/adc/enable/set",0,"on");

(
OSCFunc(
	{ |msg, time, addr, recvPort|
		msg.postln;
	},
	'/std/adc/enable',
	nil
);
)

NetAddr("picrouter.local",8080).sendMsg("/std/adc/enable/get");

NetAddr("picrouter.local",8080).sendMsg("/std/adc/enable/set",0,"off");


// get ADC value
(
OSCFunc(
	{ |msg, time, addr, recvPort|
		msg.postln;},
	'/std/adc',
	nil
);
)

NetAddr("picrouter.local",8080).sendMsg("/std/adc/enable/set",0,"on"); //on and start sending

NetAddr("picrouter.local",8080).sendMsg("/std/adc/enable/set",0,"off"); //off and stop sending

