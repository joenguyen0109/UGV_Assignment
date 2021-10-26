#include "VehicleControl.h"

int VehicleControl::connect(String^ hostName, int portNumber)
{
	Client = gcnew TcpClient(hostName, portNumber);
	// Configure connection
	Client->NoDelay = true;
	Client->ReceiveTimeout = 5000;//ms
	Client->SendTimeout = 500;//ms
	Client->ReceiveBufferSize = 1024;
	Client->SendBufferSize = 1024;
	Stream = Client->GetStream();
	return 1;
}

int VehicleControl::setupSharedMemory(SMObject& MonitorDataSMObj)
{
	hearbeatPointer = (ProcessManagement*)MonitorDataSMObj.pData;
	return 1;
}

bool VehicleControl::checkHeartBeat(long timestamp)
{
	long differenceTimeStamp = timestamp - hearbeatPointer->LifeCounter;
	if (hearbeatPointer->Shutdown.Status != 0xFF) {
		int hearBeat = (hearbeatPointer->Heartbeat.Status >> 1) & 1;
		if (!hearBeat) {
			hearbeatPointer->Heartbeat.Status |= 1UL << 1;
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

int VehicleControl::setupSharedMemory()
{
	return 0;
}

int VehicleControl::getData()
{
	return 0;
}

int VehicleControl::checkData()
{
	return 0;
}

int VehicleControl::sendDataToSharedMemory()
{
	return 0;
}

bool VehicleControl::getShutdownFlag()
{
	return false;
}

int VehicleControl::setHeartbeat(bool heartbeat)
{
	return 0;
}

void VehicleControl::sendData()
{

}

void VehicleControl::sendData(double Steering, double Speed, unsigned int flag)
{
	array<unsigned char>^ data = gcnew array<unsigned char>(50);
	array<unsigned char>^ ReadData = gcnew array<unsigned char>(100);
	Steering = -Steering;
	String^ SendControl = gcnew String("# " + Steering.ToString("f2") + " "
		+ Speed.ToString("f2") + " " + flag + " #");
	Console::WriteLine(SendControl);
	data = System::Text::Encoding::ASCII->GetBytes(SendControl);
	Stream->WriteByte(0x02);
	Stream->Write(data, 0, data->Length);
	Stream->WriteByte(0x03);
	System::Threading::Thread::Sleep(100);
	//Stream->Read(ReadData, 0, ReadData->Length);
	//String^ ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	//Console::WriteLine(ResponseData);
}

int VehicleControl::askAuth()
{
	array<unsigned char>^  data = gcnew array<unsigned char>(50);
	array<unsigned char>^ ReadData = gcnew array<unsigned char>(100);
	System::String^ Authen = gcnew System::String("5253838\n");
	data = System::Text::Encoding::ASCII->GetBytes(Authen);
	Stream->Write(data, 0, data->Length);
	Stream->Read(ReadData, 0, ReadData->Length);
	String^ ResponseData = System::Text::Encoding::ASCII->GetString(ReadData);
	Console::WriteLine(ResponseData);
	return 1;
}

VehicleControl::~VehicleControl()
{
	Stream->Close();
	Client->Close();
}
