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

//根据名称查找信号函数或者槽函数的索引(是第几个)
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

//连接信号与槽  在发送者对象中存一个map key为信号函数的索引  val为接收对象和槽函数索引
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

//发送信号
void Object::testSignal()
{
	emit signal1();
}

//根据槽函数的索引调用槽函数
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

//从发送者的connections中 取出与该信号连接的接收者和槽函数索引
void MetaObject::active(Object * sender, int index)
{
	ConnectionMapIterator it;
	std::pair<ConnectionMapIterator, ConnectionMapIterator> ret;
	ret = sender->connections.equal_range(index);
	for (it =ret.first; it!=ret.second; it++)
	{
		Connection c = (*it).second;
		//根据槽函数索引调用接收者的槽函数
		c.recever->metaCall(c.method);
	}
}

//槽函数的实现
void Object::slot1()
{
	std::cout << "slot1" << std::endl;
}

//qt自动生成  发送者为自己  这里发送的信号函数索引是0  
void Object::signal1()
{
	MetaObject::active(this, 0);
}