#include "Listener.h"
#include "EtherCard.h"


byte Ethernet::buffer[500];


// Connectes to the enc28j60 network module with a static IP address
void ListenerClass::connect(byte* ip, byte* gw, byte* mac) {
	memcpy( this->ip, ip, 4 );
	memcpy( this->gw, gw, 4 );
	memcpy( this->mac, mac, 6 );

	Serial.println( "Connecting to ethernet controller" );
	while ( ether.begin( sizeof Ethernet::buffer, this->mac ) == 0 ) {
		Serial.println( "Failed to access Ethernet controller" );
		delay( 1000 );
	}

	Serial.println( "Connected" );
	ether.staticSetup( this->ip, this->gw );
}


// Connectes to the enc28j60 network module and gets DHCP address
void ListenerClass::connect( byte* mac ) {
	memcpy( this->mac, mac, 6 );

	Serial.println( "Connecting to ethernet controller" );
	while ( ether.begin( sizeof Ethernet::buffer, this->mac ) == 0 ) {
		Serial.println( "Failed to access Ethernet controller" );
		delay( 1000 );
	}

	Serial.println( "Connected" );
	if ( !ether.dhcpSetup() )
		Serial.println( F( "DHCP failed" ) );
}


// Returns true if a packet was received
bool ListenerClass::gotPacket() {
	ether.packetLoop( packetLength );

	packetLength = ether.packetReceive();
	return packetLength > 0;
	
}


// Returns true if the last received packet was for the IP of the device
bool ListenerClass::isForMe() {
	for ( uint8_t pos = 0; pos < 4; pos++ ) {
		if ( ether.myip[pos] != *( (byte*) Ethernet::buffer + 0x1e + pos ) ) {
			return false;
		}
	}
	return true;
}


// Returns true if the last received packet was an ICMP ping request
bool ListenerClass::isIcmpPingReq() {
	if ( getProtocol() == 1 ) {
		byte *type = (byte*) Ethernet::buffer + 0x22;
		if ( *type == 8 ) return true;
	}
	return false;
}


// Returns true if the last received packet was an UDP packet
bool ListenerClass::isUdp() {
	if ( getProtocol() == 17 ) {
		return true;
	}
	return false;
}


// Returns true if the last received packet was a TCP SYN/connect packet
bool ListenerClass::isTcpSyn() {

	if ( getProtocol() == 6 ) {
		byte *flags = (byte*) Ethernet::buffer + 0x2f;
		bool syn = *flags & 0b00000010;
		if ( syn ) {
			return true;
		}
	}
	return false;
}


// Returns the protocol number of the last received packet (eg. 6=tcp, 17=udp etc)
uint8_t ListenerClass::getProtocol() {
	byte *protocol = (byte*) Ethernet::buffer + 0x17;
	return *protocol;
}


// Returns the destination port number that the last received packet was destined for
uint16_t ListenerClass::getDstPort() {
	uint8_t port_msb = *( Ethernet::buffer + 0x24 );
	uint8_t port_lsb = *( Ethernet::buffer + 0x25 );
	uint16_t dstPort = (uint16_t)port_msb * 256 + (uint16_t)port_lsb ;
	return dstPort;
}


// Returns a string with the source IP number of the last received packet
void ListenerClass::getSrcIp(char* str) {
	uint8_t b1 = *( (byte*) Ethernet::buffer + 0x1a );
	uint8_t b2 = *( (byte*) Ethernet::buffer + 0x1b );
	uint8_t b3 = *( (byte*) Ethernet::buffer + 0x1c );
	uint8_t b4 = *( (byte*) Ethernet::buffer + 0x1d );
	sprintf( str, "%d.%d.%d.%d", b1, b2, b3, b4 );
}


// Returns a string representation of an IP number in the ether library (byte array representation)
void ListenerClass::getStrIp( char* str, uint8_t* ip ) {
	sprintf( str, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3] ); 
}


// Returns a string with the mac address of the device
void ListenerClass::getMyMac( char* str ) {
	sprintf( str, "%02x:%02x:%02x:%02x:%02x:%02x", ether.mymac[0], ether.mymac[1], ether.mymac[2], ether.mymac[3], ether.mymac[4], ether.mymac[5] );
}


// Prints to serial a hex representation of the last received packet
void ListenerClass::debug() {
	char *var = (byte*) Ethernet::buffer;
	String hexString;
	for ( uint8_t i = 0; i < packetLength; i++ ) { // traverse the var byte by byte
		char* p = (char *) var;

		uint8_t currentByte = *( p + i ); // get byte number i
		char currentByteHex[3];
		sprintf( currentByteHex, "%02X", currentByte ); // convert it to hex
		hexString = hexString + currentByteHex; // and concatenate it into a printable string of all bytes

		if ( i != packetLength - 1 ) hexString = hexString + ",";
	}
	Serial.print( "Packet: " );
	Serial.println( hexString );
}