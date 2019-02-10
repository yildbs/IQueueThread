#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>
#include <thread>
#include <chrono>

#include "Modules.h"
#include "../IQueueThread.hpp"


void Sleep(int ms);
void Printf(const char* format, ...);


///////////////////////
//This program is not 'actually' working
//It is just example for how to use IQueueThread.hpp
//
///////////////////////
//Process of program
///////////////////////
//1. PacketReceiver (Input=None, Output=Packet)
//	- Receive rtsp packet
//	- Push Packet to Decoder's Queue
//
//2. Decoder (Input=Packet, Output=DecodedFrame)
//	- Pop Packet from it's own Queue
//	- Decode Packet to make DecodedFrame
//	- Push DecodedFrame to ObjectDetector
//
//3. ObjectDetector (Input=DecodedFrame, Output=ObjectList)
//	- Pop DecodedFrame from it's own Queue
//	- Detect some objects in DecodedFrame(ObjectList)
//	- Push ObjectList to ActionDetector
//
//4. ActionDetector (Input=ObjectList, Output=ActionList)
//	- Pop ObjectList from it's own Queue
//	- Judge whether some Action occured
//	- Callback to some external class function




//Input: None
//Output: RTSP Packet
class PacketReceiver : public IQueueThread<int, Packet>{
	virtual void Run(){
		for (;;){
			if (this->enabled == false) break;

			//Receive Packet
			int data = (rand() % 5 + 1) * 10; // 10, 20, 30, 40, 50
			try{

				Printf("----------------------------------------NewPacket\n%40s : %s(%d)\n", "PacketReceiver->Decoder", "Packet", data);
				//Push to next module
				this->PushNext(Packet(data));
			}
			catch (...){ }


			//Assume that you receive packets every 5 seconds
			::Sleep(5000);
		}
	}
};


class Decoder : public IQueueThread<Packet, DecodedFrame>{
	virtual void Run(){
		for (;;){
			if (this->enabled == false) break;

			Packet packet;
			this->Pop(packet);

			/////////////////
			DecodedFrame frame;

			//Decode Packet to DecodedFrame
			frame.data = packet.data + 1;

			//Assume that decoding time is 500ms
			::Sleep(500);
			/////////////////

			try{
				Printf("%40s : %s(%d)\n", "Decoder->ObjectDetector", "DecodedFrame", frame.data);
				this->PushNext(frame);
			}
			catch (...){ }
		}
	}
};


class ObjectDetector : public IQueueThread<DecodedFrame, ObjectList>{
	virtual void Run(){
		for (;;){
			if (this->enabled == false) break;

			DecodedFrame frame;
			this->Pop(frame);

			/////////////////
			ObjectList objectlist;

			//Analyze DecodedFrame
			//by using some object detection algorithm
			objectlist.data = frame.data + 1;

			//Assume that detection time is 1000ms
			::Sleep(1000);
			/////////////////

			try{
				Printf("%40s : %s(%d)\n", "ObjectDetector->ActionDetector", "ObjectList", objectlist.data);
				this->PushNext(objectlist);
			}
			catch (...){ }
		}
	}
};


class ActionDetector : public IQueueThread<ObjectList, ActionList>{
	virtual void Run(){
		for (;;){
			if (this->enabled == false) break;

			ObjectList objectlist;
			this->Pop(objectlist);

			/////////////////
			ActionList actionlist;

			//Judge whether some action occured
			actionlist.data = objectlist.data + 1;

			//Assume that detection time is 500ms
			::Sleep(500);
			/////////////////

			try{
				Printf("%40s : %s(%d)\n", "ActionDetector-> External Callback", "ActionList", actionlist.data);
				this->PushNext(actionlist);
			}
			catch (...){ }
		}
	}
};


class SomeExternalClass{
public:
	bool SomeExternalCallFunction(ActionList const &t) {
		Printf("%40s\n", t.What().c_str());
		return true;
	}
};



int main()
{
	PacketReceiver packetreceiver;
	Decoder decoder;
	ObjectDetector objectdetector;
	ActionDetector actiondetector;
	SomeExternalClass external;

	packetreceiver.SetPushNext(std::bind(&Decoder::Push, &decoder, std::placeholders::_1));
	decoder.SetPushNext(std::bind(&ObjectDetector::Push, &objectdetector, std::placeholders::_1));
	objectdetector.SetPushNext(std::bind(&ActionDetector::Push, &actiondetector, std::placeholders::_1));
	actiondetector.SetPushNext(std::bind(&SomeExternalClass::SomeExternalCallFunction, &external, std::placeholders::_1));

	packetreceiver.Start();
	decoder.Start();
	objectdetector.Start();
	actiondetector.Start();

	::Sleep(10000);//After 10 seconds, kill all threads
	
	packetreceiver.Stop();
	decoder.Stop();
	objectdetector.Stop();
	actiondetector.Stop();
}



void Sleep(int ms)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void Printf(const char* format, ...)
{
	static std::mutex mtx;
	mtx.lock();

	char dest[1024 * 16];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(dest, format, argptr);
//    vsprintf_s(dest, format, argptr);
    va_end(argptr);
    printf(dest);

	::Sleep(30);
	mtx.unlock();
}


