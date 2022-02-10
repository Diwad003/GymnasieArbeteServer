#include "Packet.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

#pragma warning(disable : 4996)

Packet::Packet()
{
	myBuffer = std::vector<BYTE*>();
}

Packet::~Packet()
{
}

std::vector<unsigned char*> Packet::GetBuffer()
{
	return this->myBuffer;
}

void Packet::ResetBuffer()
{
	for (size_t i = 0; i < myBuffer.size(); i++)
	{
		delete(myBuffer[i]);
	}
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

	myBuffer.push_back((unsigned char*)tempBufferString.data());
	myBuffer.push_back(reinterpret_cast<unsigned char*>('/'));
}

void Packet::AddTexture()
{
	cv::Mat tempImage = cv::imread("Textures/Wood.jpg");

	myBuffer.push_back(tempImage.data);
	myBuffer.push_back(reinterpret_cast <unsigned char*>('/'));
}