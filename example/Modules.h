#pragma once

#include <string>


class Packet{
public:
	Packet();
	Packet(int data);
	Packet& operator=(Packet& obj);

public:
	int data; 	// It is just example, you should use own your type
				// such as pointer, std::vector, YourClass ...
};

class DecodedFrame{
public:
	DecodedFrame();
	DecodedFrame(int data);
	DecodedFrame& operator=(DecodedFrame& obj);

public:
	int data; 	// It is just example, you should use own your type
				// such as pointer, std::vector, YourClass ...
};

class ObjectList{
public:
	ObjectList();
	ObjectList(int data);
	ObjectList& operator=(ObjectList& obj);

public:
	int data; 	// It is just example, you should use own your type
				// such as pointer, std::vector, YourClass ...
};

class ActionList{
public:
	ActionList();
	ActionList(int data);
	ActionList& operator=(ActionList& obj);
	std::string What();

public:
	int data; 	// It is just example, you should use own your type
				// such as pointer, std::vector, YourClass ...
};
