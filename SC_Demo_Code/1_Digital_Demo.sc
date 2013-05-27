// PICrouter demo code for SuperCollider
// Created by Koichiro Mori 2013.
// This content is published under CC-BY-SA 3.0 license.


thisProcess.openUDPPort(8000);// At first you should open a UDP port..


// Control Digital Port

// set digital out
NetAddr("picrouter.local",8080).sendMsg("/std/port/io/set", "b0", "out"); // set b0 pin to digial output

(
thisProcess.openUDPPort(8000);
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln;}
	, '/std/port/io',
nil);
)


// use digital out
NetAddr("picrouter.local",8080).sendMsg("/std/port/io/get", "b0"); // get i/o state of b0 pin

NetAddr("picrouter.local",8080).sendMsg("/std/port/out/set", "b0", "high"); // set b0 pin high
NetAddr("picrouter.local",8080).sendMsg("/std/port/out/set", "b0", "low"); // set b0 pin low



// set digital in
NetAddr("picrouter.local",8080).sendMsg("/std/adc/dio/set", 0, "in"); // set dio 0 pin to input

(
thisProcess.openUDPPort(8000);
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln;}
	, '/std/adc/dio',
nil);
)

NetAddr("picrouter.local",8080).sendMsg("/std/adc/dio/get", 0); // get dio stateus of 0 pin


// get digital in
(
thisProcess.openUDPPort(8000);
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln;}
	, '/std/adc/din',
nil);
)

NetAddr("picrouter.local",8080).sendMsg("/std/adc/din/get", 0); // get dio stateus of 0 pin