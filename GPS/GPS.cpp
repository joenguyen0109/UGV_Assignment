#include "GPS.h"


int GPS::connect(String^ hostName, int portNumber) 
{
	// YOUR CODE HERE
	try
	{
		Client = gcnew TcpClient(hostName, portNumber);
		// Configure connection
		Client->NoDelay = true;
		Client->ReceiveTimeout = 500;//ms
		Client->SendTimeout = 500;//ms
		Client->ReceiveBufferSize = 1024;
		Client->SendBufferSize = 1024;
		Stream = Client->GetStream();
		ReadData = gcnew array<unsigned char>(112);
	}
	catch (const std::exception&)
	{

		return 0;
	}
	return 1;
}
int GPS::setupSharedMemory() 
{
	SMObject GPSDataSMObj(_TEXT("GPSData"), sizeof(GPSData));
	dataPtr = NULL;
	GPSDataSMObj.SMCreate();
	GPSDataSMObj.SMAccess();
	dataPtr = (GPSData*)GPSDataSMObj.pData;
	// YOUR CODE HERE
	return 1;
}
int GPS::getData() 
{
	// YOUR CODE HERE
	
	Stream->Read(ReadData, 0, ReadData->Length);
	Console::WriteLine(BitConverter::ToString(ReadData));
	//Console::WriteLine(BitConverter::ToDouble(ReadData, 16+28));
	//Console::WriteLine(BitConverter::ToDouble(ReadData, 24+28));
	//Console::WriteLine(BitConverter::ToDouble(ReadData, 32+28));
	return 1;
}
int GPS::checkData()
{
	// YOUR CODE HERE
	unsigned long checksum = CRC32Value(BitConverter::ToInt32(ReadData, 108));
	//unsigned char s[80];
	//int k = 28;
	//for (int i = 0; i < 80; i++) {
	//	s[i] = ReadData[k + i];
	//}
	pin_ptr<unsigned char>buff = &ReadData[0];
	Console::WriteLine(checksum == CalculateBlockCRC32(80,buff));
	return 1;
}
int GPS::sendDataToSharedMemory() 
{
	// YOUR CODE HERE
	return 1;
}
bool GPS::getShutdownFlag() 
{
	// YOUR CODE HERE
	return 1;
}
int GPS::setHeartbeat(bool heartbeat) 
{
	// YOUR CODE HERE
	return 1;
}
GPS::~GPS()
{
	Stream->Close();
	Client->Close();
	// YOUR CODE HERE
}

unsigned long CRC32Value(int i)
{
	int j;
	unsigned long ulCRC;
	ulCRC = i;
	for (j = 8; j > 0; j--)
	{
		if (ulCRC & 1)
			ulCRC = (ulCRC >> 1) ^ CRC32_POLYNOMIAL;
		else
			ulCRC >>= 1;
	}
	return ulCRC;
}

unsigned long CalculateBlockCRC32(unsigned long ulCount, /* Number of bytes in the data block */
	unsigned char* ucBuffer) /* Data block */
{
	unsigned long ulTemp1;
	unsigned long ulTemp2;
	unsigned long ulCRC = 0;
	while (ulCount-- != 0)
	{
		ulTemp1 = (ulCRC >> 8) & 0x00FFFFFFL;
		ulTemp2 = CRC32Value(((int)ulCRC ^ *ucBuffer++) & 0xff);
		ulCRC = ulTemp1 ^ ulTemp2;
	}
	return(ulCRC);
}
