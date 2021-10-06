#include "Laser.h"


int Laser::connect(String^ hostName, int portNumber)
{
	Client = gcnew TcpClient(hostName, portNumber);
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 5000;//ms
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
	System::Threading::Thread::Sleep(100);
}

int Laser::askAuth()
{
	array<unsigned char>^ SendData = gcnew array<unsigned char>(16);
	String^ AskScan = gcnew String("5253838\n");
	SendData = System::Text::Encoding::ASCII->GetBytes(AskScan);
	Stream->Write(SendData, 0, SendData->Length);
	System::Threading::Thread::Sleep(10);
	Stream->Read(ReadData, 0, ReadData->Length);
	String ^ ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	Console::WriteLine(ResponseData);
	return 1;
}

int Laser::getData()
{

	Stream->Read(ReadData, 0, ReadData->Length);
	String  ^ ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	//Console::WriteLine(ResponseData);
	array<String^>^ data = ResponseData->Split(' ');
	Console::WriteLine("Start Angle: {0:F}\n", Convert::ToInt32(data[23], 16));
	Console::WriteLine("Angular Step: {0:F}\n", Convert::ToUInt16(data[24], 16)/10000.0);
	Console::WriteLine("Number of data: {0:F}\n", Convert::ToUInt16(data[25], 16));
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
	String^ ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	Console::WriteLine(ResponseData);
	array<String^>^ data = ResponseData->Split(' ');
	UInt16 numberOfData = Convert::ToUInt16(data[25], 16);
	UInt16 resolution = Convert::ToUInt16(data[24], 16);
	for (int i = 0; i < numberOfData; i++) {
		UInt16 output = Convert::ToUInt16(data[26 + i], 16);
		double angle = (double) (i*resolution) * (Math::PI / 180);
		dataPtr->x[i] = (double)output * Math::Sin(angle);
		dataPtr->y[i] =	-(double)output * Math::Cos(angle);
	}
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
