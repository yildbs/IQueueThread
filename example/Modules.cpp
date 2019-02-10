#include "Modules.h"


/* Packet */
Packet::Packet() 
{ 
}

Packet::Packet(int data)
	: data(data)
{ 
}

Packet& Packet::operator=(Packet& obj)
{
	this->data = obj.data;
	return *this;
}


/* DecodedFrame */
DecodedFrame::DecodedFrame()
{ 
}

DecodedFrame::DecodedFrame(int data)
	: data(data)
{ 
}

DecodedFrame& DecodedFrame::operator=(DecodedFrame& obj)
{
	this->data = obj.data;
	return *this;
}


/* ObjectList */
ObjectList::ObjectList()
{ 
}
ObjectList::ObjectList(int data)
	: data(data)
{ 
}
ObjectList& ObjectList::operator=(ObjectList& obj)
{
	this->data = obj.data;
	return *this;
}


/* ActionList */
ActionList::ActionList()
{ 
}

ActionList::ActionList(int data)
	: data(data)
{ 
}

ActionList& ActionList::operator=(ActionList& obj)
{
	this->data = obj.data;
	return *this;
}

std::string ActionList::What()
{
	return std::string("Action detected : ") + std::to_string(this->data);
}

