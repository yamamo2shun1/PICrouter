// PICrouter demo code for SuperCollider
// Created by Koichiro Mori 2013.
// This content is published under CC-BY-SA 3.0 license.


thisProcess.openUDPPort(8000); // At first you should open a UDP port..

n = NetAddr("picrouter.local",8080); // address

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

n.sendMsg("/sys/host/ip/get"); // get host ip


// set remote ip
n.sendMsg("/sys/remote/ip/set", /*"<YOUR MACHINE'S IP>"*/);


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

n.sendMsg("/sys/remote/ip/get"); // get remote ip

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
n.sendMsg("/std/onboard/led", 0, "on"); // on

n.sendMsg("/std/onboard/led", 0, "off"); // off


n.sendMsg("/std/onboard/led", 1, "on"); // on

n.sendMsg("/std/onboard/led", 1, "off"); // off
