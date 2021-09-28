#include "Laser.h"


int Laser::connect(String^ hostName, int portNumber)
{
	Client = gcnew TcpClient(hostName, portNumber);
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 500;//ms
	Client->SendTimeout = 500;//ms
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;
	Stream = Client->GetStream();
	ReadData = gcnew array<unsigned char>(2500);
	return 1;
}
int Laser::setupSharedMemory(SMObject& LaserDataSMObj, SMObject& MonitorDataSMObj)
{
	dataPtr = (SM_Laser*)LaserDataSMObj.pData;
	hearbeatPointer = (ProcessManagement*)MonitorDataSMObj.pData;
	return 1;
}
bool Laser::checkHeartBeat(long timestamp) {
	long differenceTimeStamp = timestamp - hearbeatPointer->LifeCounter;
	if (hearbeatPointer->Shutdown.Status != 0xFF) {
		int hearBeat = (hearbeatPointer->Heartbeat.Status >> 0) & 1;
		if (!hearBeat) {
			hearbeatPointer->Heartbeat.Status |= 1UL << 0;
			Console::WriteLine("Flip");
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
int Laser::setupSharedMemory()
{

	// YOUR CODE HERE
	return 1;
}

void Laser::sendData() {
	array<unsigned char>^ SendData = gcnew array<unsigned char>(16);
	String^ AskScan = gcnew String("sRN LMDscandata");
	SendData = System::Text::Encoding::ASCII->GetBytes(AskScan);
	Stream->WriteByte(0x02);
	Stream->Write(SendData, 0, SendData->Length);
	Stream->WriteByte(0x03);
	System::Threading::Thread::Sleep(10);
}

int Laser::getData()
{

	Stream->Read(ReadData, 0, ReadData->Length);
	Console::WriteLine(BitConverter::ToString(ReadData));
	return 1;
}
int Laser::checkData()
{
	// YOUR CODE HERE

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
