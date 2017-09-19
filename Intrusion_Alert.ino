#include <Arduino.h>
#include "ScrollDisplay.h"
#include "Listener.h"
#include "EtherCard.h"
#include "Wire.h"


// Here you setup static IP or DHCP
static bool dhcp = true;
static byte myip[] = { 192,168,80,2 }; // Only used if you want static IP
static byte gwip[] = { 192,168,80,1 }; // Only used if you want static IP
static byte mymac[] = { 0xDE,0xAD,0xBE,0xEF,0x00,0x01 }; // Set this to something clever.


// Global objects
ListenerClass listener;
ScrollDisplay disp( 4 );


void setup()
{
	Wire.begin();
	Serial.begin( 115200 );
	if ( dhcp ) listener.connect( mymac );
	if (!dhcp)  listener.connect(myip,gwip,mymac);
	disp.setup( 0x3C );
	showInfo();
}


void loop() {
	static char msg[20];
	if ( listener.gotPacket() ) {
		if ( listener.isForMe() ) {
			if ( listener.isUdp() ) sprintf( msg, "UDP port %d src=", listener.getDstPort() );
			if ( listener.isTcpSyn() )sprintf( msg, "TCP port %d src=", listener.getDstPort() );
			if ( listener.isIcmpPingReq() ) sprintf( msg, "ICMP src=" );
			disp.printScrollLine( msg );

			listener.getSrcIp( msg );
			disp.printScrollLine( msg );
		}
	}
	disp.handle();
}


void showInfo() {
	char info[20];
	listener.getMyMac( info );
	disp.printScrollLine( info );
	listener.getStrIp( info, ether.myip );
	disp.printScrollLine( info );
	listener.getStrIp( info, ether.netmask );
	disp.printScrollLine( info );
	listener.getStrIp( info, ether.gwip );
	disp.printScrollLine( info );

	disp.blankAfter( 20 );
}