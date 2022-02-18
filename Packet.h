#ifndef PACKET_HEADER
#define PACKET_HEADER

#include <WS2tcpip.h>
#include <vector>
#include <iostream>
#include <string>

class Packet
{
public:
	Packet();
	~Packet();

	std::vector<std::string> GetBuffer();
	void ResetBuffer();

	void AddSystemTime();
	void AddLevel1();

private:
	std::vector<std::string> myBuffer;
};
#endif