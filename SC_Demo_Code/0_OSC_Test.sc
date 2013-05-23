// PICrouter demo code for SuperCollider
// Created by Koichiro Mori 2013.
// This content is published under CC-BY-SA 3.0 license.


thisProcess.openUDPPort(8000);// At first you should open a UDP port..

// get host ip
(
OSCFunc(
	{ |msg, time, addr, recvPort|
		msg.postln;
	},
	'/sys/host/ip',
	nil
);
)

NetAddr("picrouter.local",8080).sendMsg("/sys/host/ip/get");


// set remote ip
NetAddr("picrouter.local",8080).sendMsg("/sys/remote/ip/set", /*"<YOUR MACHINE'S IP>"*/);


// get remote ip
(
OSCFunc(
	{ |msg, time, addr, recvPort|
		msg.postln
	},
	"/sys/remote/ip",
	nil
);
)

NetAddr("picrouter.local",8080).sendMsg("/sys/remote/ip/get");

// get switche
(
thisProcess.openUDPPort(8000);
OSCFunc(
	{ |msg, time, addr, recvPort|
		msg.postln;
	},
	'/std/onboard/sw1',
	nil
);
)

// LED on/off
NetAddr("picrouter.local",8080).sendMsg("/std/onboard/led",0,"on");

NetAddr("picrouter.local",8080).sendMsg("/std/onboard/led",0,"off");


NetAddr("picrouter.local",8080).sendMsg("/std/onboard/led",1,"on");

NetAddr("picrouter.local",8080).sendMsg("/std/onboard/led",1,"off");
