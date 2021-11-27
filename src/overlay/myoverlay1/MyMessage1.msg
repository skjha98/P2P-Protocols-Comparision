cplusplus {{
	#include <TransportAddress.h>
}}
class noncoobject TransportAddress;

enum MessageType{
	MYMSG_PING = 1;
	MYMSG_PONG = 2;
}

packet MyMessage1{
	int type enum (MessageType);
	TransportAddress senderAddress;
}

