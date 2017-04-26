//===---------------------------------------------------------------------===//
//	Copyright 2013 Leaning Technlogies
//===----------------------------------------------------------------------===//

#include <tests.h>

class A
{
public:
	int a;
	A():a(42)
	{
		cheerp::console_log("Static initialiation of class instances 1/3",": SUCCESS");
	}
};

int f()
{
	static A a;
	a.a++;
	return a.a;
}

void webMain()
{
	assertEqual(f(), 43, "Static initialiation of class instances 2/3");
	assertEqual(f(), 44, "Static initialiation of class instances 3/3");
}
