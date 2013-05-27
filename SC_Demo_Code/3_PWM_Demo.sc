// PICrouter demo code for SuperCollider
// Created by Koichiro Mori 2013.
// This content is published under CC-BY-SA 3.0 license.


thisProcess.openUDPPort(8000);// At first you should open a UDP port..

// Control PWM
NetAddr("picrouter.local",8080).sendMsg("/std/pwm/enable/set", 0, "on"); // set pwm pin 0 to on

(
thisProcess.openUDPPort(8000);
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln;}
	, '/std/pwm/enable',
nil);
)

NetAddr("picrouter.local",8080).sendMsg("/std/pwm/enable/get", 0); // get enable status of pwm pin 0


// pwm duty cycle
NetAddr("picrouter.local",8080).sendMsg("/std/pwm/duty/set", 0, 50); // set duty cycle to 50

(
thisProcess.openUDPPort(8000);
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln;}
	, '/std/pwm/duty',
nil);
)

NetAddr("picrouter.local",8080).sendMsg("/std/pwm/duty/get", 0);


// pwm frequency
NetAddr("picrouter.local",8080).sendMsg("/std/pwm/freq/set", 440); // set frequency to 440

(
thisProcess.openUDPPort(8000);
OSCFunc({
	|msg, time, addr, recvPort|
	msg.postln;}
	, '/std/pwm/freq',
nil);
)

NetAddr("picrouter.local",8080).sendMsg("/std/pwm/freq/get", 0);
