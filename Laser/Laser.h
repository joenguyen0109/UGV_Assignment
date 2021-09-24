#pragma once

#include <UGV_module.h>

#include <smstructs.h>
#using <System.dll>
#include <stdio.h>

using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text;

struct LaserData {
	double X;
	double Y;
};


ref class Laser : public UGV_module
{


public:
	int connect(String^ hostName, int portNumber) override;
	int setupSharedMemory(SMObject&);
	int setupSharedMemory() override;
	int getData() override;
	int checkData() override;
	int sendDataToSharedMemory() override;
	bool getShutdownFlag() override;
	int setHeartbeat(bool heartbeat) override;
	void sendData();
	~Laser();

protected:
	// YOUR CODE HERE (ADDITIONAL MEMBER VARIABLES THAT YOU MAY WANT TO ADD)
	TcpClient^ Client;
	NetworkStream^ Stream;
	array<Byte>^ ReadData;
	LaserData* dataPtr;

};


