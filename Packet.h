#ifndef PACKET_HEADER
#define PACKET_HEADER

#include <WS2tcpip.h>
#include <vector>

class Packet
{
public:
	Packet();
	~Packet();

	std::vector<unsigned char*> GetBuffer();
	void ResetBuffer();

	void AddSystemTime();
	void AddTexture();

private:
	std::vector<unsigned char*> myBuffer;
};
#endif