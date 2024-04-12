#ifndef HOOK_H_
#define HOOK_H_

#include <cstdio>

#include <Windows.h>

namespace Hook
{
	inline bool initialize = false;
	bool Init();
	bool GetState();
	void Shutdown();
}

#endif