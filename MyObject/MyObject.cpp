// MyObject.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Object.h"

int main()
{
	Object obj1, obj2;
	obj1.connect(&obj1, "sig1", &obj2, "slot1");
	obj1.testSignal();

	system("pause");

    return 0;
}

