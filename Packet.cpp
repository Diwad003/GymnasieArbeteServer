#include "Packet.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

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

	myBuffer.push_back("SystemTime");
	myBuffer.push_back("/");
	myBuffer.push_back(tempBufferString.data());
	myBuffer.push_back("/");
}

void Packet::AddTexture()
{
	cv::Mat tempImage = cv::imread("Textures/Wood.jpg");

	myBuffer.push_back("Texture");
	myBuffer.push_back("/");
	myBuffer.push_back(reinterpret_cast<char*>(tempImage.data));
	myBuffer.push_back("/");
}