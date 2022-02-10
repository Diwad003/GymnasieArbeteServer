#include "Networking.h"
#include <sstream>
#include <iostream>
#include <algorithm>

#pragma comment (lib, "ws2_32.lib")

Networking::Networking()
{
	myClientSocket = SOCKET();
	myListening = SOCKET();
	myPacket = new Packet();
}

Networking::~Networking()
{
	closesocket(myClientSocket);
	closesocket(myListening);
	WSACleanup();
}

void Networking::StartServer()
{
	closesocket(myClientSocket);
	closesocket(myListening);
	WSACleanup();

	//Initialze windsock
	WSADATA tempWsData;
	WORD tempVer = MAKEWORD(2, 2);

	int tempWsOK = WSAStartup(tempVer, &tempWsData);
	if (tempWsOK != 0)
	{
		printf("WAS is INVALID");
		return;
	}

	//Create a Socket
	myListening = socket(AF_INET, SOCK_STREAM, 0);
	printf("Listening socket created\n");
	if (myListening == INVALID_SOCKET)
	{
		printf("Listening socket is INVALID\n");
		return;
	}


	//Bind the socket to an ip adress and port
	sockaddr_in tempAddr;
	tempAddr.sin_family = AF_INET;
	tempAddr.sin_port = htons(54000);
	tempAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	bind(myListening, (sockaddr*)&tempAddr, sizeof(tempAddr));

	//Get IP
	GetIP(tempAddr);

	//Tell windsock the socket is for listening
	listen(myListening, SOMAXCONN);
	printf("Listening for connections\n");

	ClientConnect();
	RequestLoop();
}

void Networking::ClientConnect()
{
	// Wait for a connection
	sockaddr_in tempClientSocketADDR;
	int tempClientSize = sizeof(tempClientSocketADDR);
	myClientSocket = accept(myListening, (sockaddr*)&tempClientSocketADDR, &tempClientSize);
	std::cout << "Client has connected : Client IP : " << (std::string)GetIP(tempClientSocketADDR) << "\n";
	if (myClientSocket == INVALID_SOCKET)
	{
		printf("Client socket is INVALID: Returning\n");
		return;
	}

	char tempHost[NI_MAXHOST];
	char tempService[NI_MAXHOST];
	ZeroMemory(tempHost, NI_MAXHOST);
	ZeroMemory(tempService, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&tempClientSocketADDR, sizeof(tempClientSocketADDR), tempHost, NI_MAXHOST, tempService, NI_MAXSERV, 0) == 0)
	{
		std::ostringstream tempOStringStream;
		tempOStringStream << tempHost << " connected in port " << tempService;
		std::cout << tempOStringStream.str() << "\n";
	}
	else
	{
		inet_ntop(AF_INET, &tempClientSocketADDR.sin_addr, tempHost, NI_MAXHOST);

		std::ostringstream tempOStringStream;
		tempOStringStream << tempHost << " connected in port " << ntohs(tempClientSocketADDR.sin_port);
		std::cout << tempOStringStream.str() << "\n";
	}
}

char* Networking::GetIP(sockaddr_in aSockAddr)
{
	const char* tempGoogleDNSServer = "8.8.8.8";
	int tempPort = 53;

	struct sockaddr_in tempService;
	int tempSock = socket(AF_INET, SOCK_DGRAM, 0);

	//Socket could not be created
	if (tempSock < 0)
	{
		std::cout << "Socket error" << std::endl;
	}

	memset(&tempService, 0, sizeof(tempService));
	tempService.sin_family = AF_INET;
	tempService.sin_addr.s_addr = inet_pton(tempService.sin_family, tempGoogleDNSServer, &tempService.sin_addr);
	tempService.sin_port = htons(tempPort);

	int tempErrorCode = connect(tempSock, (const struct sockaddr*)&tempService, sizeof(tempService));
	if (tempErrorCode < 0)
	{
		std::cout << "Error number: " << errno << ". Error message: " << std::to_string(errno) << std::endl;
	}

	struct sockaddr_in tempName;
	socklen_t namelen = sizeof(tempName);
	tempErrorCode = getsockname(tempSock, (struct sockaddr*)&tempName, &namelen);

	char tempIPBuffer[80];
	const char* tempInetChar = inet_ntop(AF_INET, &tempName.sin_addr, tempIPBuffer, 80);
	if (tempInetChar != NULL)
	{
		std::cout << "my local IP address is: " << tempIPBuffer << std::endl;
	}
	else
	{
		std::cout << "Error number: " << errno << ". Error message: " << std::to_string(errno) << std::endl;
	}

	closesocket(tempSock);
	return tempIPBuffer;
}

void Networking::SendBufferToClient()
{
	std::vector<unsigned char*> tempBufferBytes = myPacket->GetBuffer();
	for (size_t i = 0; i < tempBufferBytes.size(); i++)
	{
		send(myClientSocket, reinterpret_cast<char*>(tempBufferBytes[i]), sizeof(tempBufferBytes[i]), 0);
	}


	for (int i = 0; i < tempBufferBytes.size(); i++)
	{
		delete tempBufferBytes[i];
	}
	tempBufferBytes.clear();
}

BOOL Networking::RequestLoop()
{
	//Wait for client to send data
	char tempMessageChar[MAXCHAR];
	ZeroMemory(tempMessageChar, MAXCHAR);
	printf("Receive Request from Client\n");
	int tempBytesCount = recv(myClientSocket, tempMessageChar, sizeof tempMessageChar, 0);
	if (tempBytesCount == SOCKET_ERROR || tempBytesCount == 0)
	{
		printf("Lost connection to Client\n");
		return NULL;
	}


	std::string tempReceivedString;
	for (size_t i = 0; i < tempBytesCount; i++)
		tempReceivedString += tempMessageChar[i];

	myPacket->ResetBuffer();
	myPacket->AddSystemTime();
	if (tempReceivedString == "Texture")
	{
		myPacket->AddTexture();
	}
	if (tempReceivedString == "LoadLevel1")
	{
		//std::string tempLevelInformation = myFileManager->ReadFromFile("TextFiles/Level1.txt");
		//std::string tempSplitter = "|";
		//size_t tempPos = 0;
		//std::string tempToken;
		//std::vector<std::string> tempGameObjectData = std::vector<std::string>();
		//while ((tempPos = tempLevelInformation.find(tempSplitter)) != std::string::npos) 
		//{
		//	tempToken = tempLevelInformation.substr(0, tempPos);
		//	tempGameObjectData.push_back(tempToken);
		//	tempLevelInformation.erase(0, tempPos + tempSplitter.length());
		//}
		//float tempPosition[3] = { std::stof(tempGameObjectData[1]), std::stof(tempGameObjectData[2]), std::stof(tempGameObjectData[3]) };
		//float tempRotation[3] = { std::stof(tempGameObjectData[4]), std::stof(tempGameObjectData[5]), std::stof(tempGameObjectData[6]) };
		//Gameobject tempGameobject = Gameobject(tempGameObjectData[0], tempPosition, tempRotation);
		//myPacket->AddGameobject(tempGameobject);
		//SendBufferToClient();
	}
	SendBufferToClient();

	return RequestLoop();
}