#ifndef _NETWORK_h
#define _NETWORK_h

#include <Arduino.h>

class ListenerClass {

protected:
	byte ip[4];
	byte gw[4];
	byte mac[6];
	int packetLength = 0;

public:
	void connect( byte * mac );
	void connect( byte * ip, byte * gw, byte * mac );
	void debug();

	bool gotPacket();
	bool isForMe();
	bool isIcmpPingReq();
	bool isUdp();
	bool isTcpSyn();
	uint8_t getProtocol();
	uint16_t getDstPort();
	void getSrcIp(char* str);
	void getStrIp( char * str, uint8_t * ip );
	void getMyMac( char * str );
};

#endif

