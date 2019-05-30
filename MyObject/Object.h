#pragma once

#include <map>

class Object;
struct MetaObject
{
	const char* signalsNames;
	const char* slotsNames;
	static void active(Object* sender, int index);
};

struct Connection 
{
	Object* recever;
	int method;
};

typedef std::multimap<int, Connection> ConnectionMap;
typedef std::multimap<int, Connection>::iterator ConnectionMapIterator;


#define slots
#define signals public
#define emit 


class Object
{
public:
	Object();
	virtual ~Object();
	static void connect(Object*, const char*, Object*, const char*);
	void testSignal();

signals:
	void signal1();

public slots:
void slot1();

friend class MetaObject;

private:
	static MetaObject meta;
	void metaCall(int index);
	ConnectionMap connections;

};

