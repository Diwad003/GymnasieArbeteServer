#include "Packet.h"
#pragma warning(disable : 4996)

Packet::Packet()
{
	myBuffer = std::vector<std::string>();
}

Packet::~Packet()
{
}

std::vector<std::string> Packet::GetBuffer()
{
	return this->myBuffer;
}

void Packet::ResetBuffer()
{
	myBuffer.clear();
}

void Packet::AddSystemTime()
{
	time_t tempNow = time(0);
	int tempBufferSize = 30;
	char* tempBuffer = new char[tempBufferSize];
	ZeroMemory(tempBuffer, sizeof(tempBuffer));
	ctime_s(tempBuffer, tempBufferSize, &tempNow);
	std::string tempBufferString = tempBuffer;
	tempBufferString.erase(std::remove(tempBufferString.begin(), tempBufferString.end(), '\n'), tempBufferString.end());
	delete[] tempBuffer;

	myBuffer.push_back("SystemTime");
	myBuffer.push_back("/");
	myBuffer.push_back(tempBufferString.data());
	myBuffer.push_back("/");
}

void Packet::AddLevel1()
{
	myBuffer.push_back("Texture");
	myBuffer.push_back("/");
	myBuffer.push_back("Wood");
	myBuffer.push_back("/");

	myBuffer.push_back("GameObjects");
	myBuffer.push_back("/");
	myBuffer.push_back("2:2:2");
	myBuffer.push_back("/");
	myBuffer.push_back("Cube");
	myBuffer.push_back("/");
}