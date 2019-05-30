#include "stdafx.h"
#include "Object.h"
#include <string>
#include <iostream>

static const char sig_names[] = "sig1\n";
static const char slts_names[] = "slot1\n";
MetaObject Object::meta = { sig_names, slts_names };

Object::Object()
{

}

Object::~Object()
{
}

static int findString(const char* str, const char* subStr)
{
	if (strlen(str) < strlen(subStr))
	{
		return -1;
	}
	int index = 0;
	int len = strlen(subStr);
	bool start = true;
	const char* pos = str;
	while (*pos)
	{
		if (start && !strncmp(pos, subStr, len) && pos[len]=='\n')
		{
			return index;
		}
		start = false;
		if (*pos=='\n')
		{
			index++;
			start = true;
		}
		pos++;
	}
	return -1;
}

void Object::connect(Object *sender, const char *signal, Object *receiver, const char *slot)
{
	int signalIndex = findString(sender->meta.signalsNames, signal);
	int slotIndex = findString(receiver->meta.slotsNames, slot);
	if (signalIndex==-1 || slotIndex==-1)
	{
		std::cout << "signal or slot not found" << std::endl;
	}
	else
	{
		Connection c = { receiver , slotIndex };
		sender->connections.insert(std::pair<int, Connection>(signalIndex, c));
	}
}

void Object::testSignal()
{
	emit signal1();
}

void Object::metaCall(int index)
{
	switch (index)
	{
	case 0:
		slot1();
		break;
	default:
		break;
	}
}

void MetaObject::active(Object * sender, int index)
{
	ConnectionMapIterator it;
	std::pair<ConnectionMapIterator, ConnectionMapIterator> ret;
	ret = sender->connections.equal_range(index);
	for (it =ret.first; it!=ret.second; it++)
	{
		Connection c = (*it).second;
		c.recever->metaCall(c.method);
	}
}


void Object::slot1()
{
	std::cout << "slot1" << std::endl;
}

void Object::signal1()
{
	MetaObject::active(this, 0);
}