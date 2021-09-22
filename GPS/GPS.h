
#include <UGV_module.h>
#include <smstructs.h>
#using <System.dll>
#include <stdio.h>
#define CRC32_POLYNOMIAL 0xEDB88320L

unsigned long CRC32Value(int i);
unsigned long CalculateBlockCRC32(unsigned long ulCount, unsigned char* ucBuffer);
using namespace System;
using namespace System::Net::Sockets;
using namespace System::Net;
using namespace System::Text; 

struct GPSData {
	double X;
	double Y;
	double height;
};


ref class GPS : public UGV_module
{


public:
	int connect(String^ hostName, int portNumber) override;
	int setupSharedMemory() override;
	int getData() override;
	int checkData() override;
	int sendDataToSharedMemory() override;
	bool getShutdownFlag() override;
	int setHeartbeat(bool heartbeat) override;
	~GPS();

protected:
	// YOUR CODE HERE (ADDITIONAL MEMBER VARIABLES THAT YOU MAY WANT TO ADD)
	TcpClient^ Client;
	NetworkStream^ Stream;
	GPSData* dataPtr;
	array<Byte>^ ReadData;
};


