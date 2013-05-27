// PICrouter demo code for SuperCollider
// Created by Koichiro Mori 2013.
// This content is published under CC-BY-SA 3.0 license.


thisProcess.openUDPPort(8000);// At first you should open a UDP port..

n = NetAddr("picrouter.local",8080); // address

// System Configurations

// set and get host name
n.sendMsg("/sys/host/name/set", "osc.local");

(
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln;
	n = NetAddr(msg[1], 8080);
	}
	, '/sys/host/name',
nil);
)

n.sendMsg("/sys/host/name/get"); // get host name


//get host port number
(
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln;}
	, '/sys/host/port',
nil);
)

n.sendMsg("/sys/host/port/get"); // get port number


// set and get remote ip
n.sendMsg("/sys/remote/ip/set",  /*"<YOUR REMOTE IP>"*/); // set remote ip

(
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln}
	, "/sys/remote/ip",
nil);
)

n.sendMsg("/sys/remote/ip/get"); // get remote ip


// set and get remote port
n.sendMsg("/sys/remote/port/set", "5000"); // set remote port 5000

(
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln}
	, "/sys/remote/port",
nil);
)

n.sendMsg("/sys/remote/port/get"); // get remote port


// set and get prefix
n.sendMsg("/sys/prefix/set", "osc");

(
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln}
	, "/sys/prefix",
nil);
)

n.sendMsg("/sys/prefix/get");


// reset software
n.sendMsg("/sys/soft/reset", "normal");


//not completed.. 2013/05/27
