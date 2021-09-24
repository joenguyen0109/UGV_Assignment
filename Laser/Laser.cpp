#include "Laser.h"


int Laser::connect(String^ hostName, int portNumber)
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
		ReadData = gcnew array<Byte>(112);
	}
	catch (const std::exception&)
	{

		return 0;
	}
	return 1;
}
int Laser::setupSharedMemory(SMObject& GPSDataSMObj)
{
	
	// YOUR CODE HERE
	return 1;
}
int Laser::setupSharedMemory()
{

	// YOUR CODE HERE
	return 1;
}
int Laser::getData()
{
	// YOUR CODE HERE

	Stream->Read(ReadData, 0, ReadData->Length);
	Console::WriteLine(BitConverter::ToString(ReadData));
	return 1;
}
int Laser::checkData()
{
	// YOUR CODE HERE
	unsigned long checksum = CRC32Value(BitConverter::ToInt32(ReadData, 108));
	array<Byte>^ copy = gcnew array<Byte>(81);
	Array::Copy(ReadData, 28, copy, 0, 80);
	copy[80] = '\0';
	pin_ptr<Byte>buff = &copy[0];
	unsigned char* cp = buff;
	printf_s("%s\n", cp);
	unsigned long checksum2 = CalculateBlockCRC32(81, cp);
	std::cout << checksum << "\n";
	std::cout << checksum2 << "\n";
	std::cout << "-------------" << "\n";
	//array<Byte>^ arr = gcnew array<Byte>(6);
	//arr[0] = 'C';
	//arr[1] = '+';
	//arr[2] = '+';
	//arr[3] = 's';
	//arr[4] = 'd';
	//arr[5] = '\0';
	//pin_ptr<Byte> p = &arr[0];   // entire array is now pinned
	//unsigned char* cp = p;

	//printf_s("%s\n", cp); // bytes pointed at by cp
						  // will not move during call
	return 1;
}
int Laser::sendDataToSharedMemory()
{
	// YOUR CODE HERE

	return 1;
}
bool Laser::getShutdownFlag()
{
	// YOUR CODE HERE
	return 1;
}
int Laser::setHeartbeat(bool heartbeat)
{
	// YOUR CODE HERE
	return 1;
}
Laser::~Laser()
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
