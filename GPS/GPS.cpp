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
		ReadData = gcnew array<Byte>(112);
	}
	catch (const std::exception&)
	{

		return 0;
	}
	return 1;
}
int GPS::setupSharedMemory(SMObject & GPSDataSMObj, SMObject& MonitorDataSMObj)
{
	dataPtr = (SM_GPS*)GPSDataSMObj.pData;
	hearbeatPointer = (ProcessManagement*)MonitorDataSMObj.pData;
	return 1;
}

bool GPS::checkHeartBeat(long timestamp) {
	long differenceTimeStamp = timestamp - hearbeatPointer->LifeCounter;
	if (hearbeatPointer->Shutdown.Status != 0xFF) {
		int hearBeat = (hearbeatPointer->Heartbeat.Status >> 2) & 1;
		if (!hearBeat) {
			hearbeatPointer->Heartbeat.Status |= 1UL << 2;
		}
		else {
			//check timestamp
			if (differenceTimeStamp > 5000) {
				return true;
			}
		}
		return false;
	}
	return true;
}
int GPS::setupSharedMemory()
{
	return 1;
}
int GPS::getData() 
{
	Stream->Read(ReadData, 0, ReadData->Length);
	Console::WriteLine(BitConverter::ToString(ReadData));
	Console::WriteLine(BitConverter::ToDouble(ReadData, 16 + 28));
	Console::WriteLine(BitConverter::ToDouble(ReadData, 24 + 28));
	Console::WriteLine(BitConverter::ToDouble(ReadData, 32 + 28));
	unsigned long crc = BitConverter::ToUInt32(ReadData, 108);
	swap(crc);
	std::cout << std::hex << crc << std::endl;
	Console::WriteLine("-------------");
	return 1;
}
int GPS::checkData()
{
	unsigned char data[108];
	for (int i = 0; i < 108; i++) {
		data[i] = ReadData[i];
	}
	unsigned long crc = CalculateBlockCRC32(108, data);
	if (crc == BitConverter::ToUInt32(ReadData, 108)) {
		return 1;
	}
	return 0;
}
int GPS::sendDataToSharedMemory() 
{
	// YOUR CODE HERE
	dataPtr->northing = BitConverter::ToDouble(ReadData, 16 + 28);
	dataPtr->easting = BitConverter::ToDouble(ReadData, 24 + 28);
	dataPtr->height = BitConverter::ToDouble(ReadData, 32 + 28);
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

unsigned long CalculateBlockCRC32(
	unsigned long ulCount, /* Number of bytes in the data block */
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

void swap(unsigned long& val) {
	val = ((val << 24) & 0xFF000000) |
		((val << 8) & 0x00FF0000) |
		((val >> 8) & 0x0000FF00) |
		((val >> 24) & 0x000000FF);
}