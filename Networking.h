#ifndef NETWORKING_HEADER
#define NETWORKING_HEADER

#include "Packet.h"

class Networking
{
public:
	Networking();
	~Networking();

	void StartServer();
	void ClientConnect();
	char* GetIP(sockaddr_in aSockAddr);
	void SendBufferToClient();
	BOOL RequestLoop();

private:
	SOCKET myClientSocket;
	SOCKET myListening;
	Packet* myPacket;
};
#endif