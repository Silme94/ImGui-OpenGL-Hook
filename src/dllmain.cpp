#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "MinHook.h"
#include "hook.h"
#include "gui.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_win32.h"
#include "../opengl/include/glad/glad.h"


HINSTANCE DllHandle;

DWORD __stdcall EjectThread() {
	Sleep(100);
	FreeLibraryAndExitThread(DllHandle, EXIT_SUCCESS);
	return 0;
}

void ShutdownThread(FILE* hfile, char* err_msg = "Exiting with no error message definied") {
	std::cerr << err_msg << "\n";
	Sleep(100);
	GUI::Shutdown();
	Hook::Shutdown();
	FreeConsole();
	if (hfile == nullptr)
		fclose(hfile);

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EjectThread, NULL, 0, NULL);
}


void MainThread() {

	AllocConsole();

	FILE* file = nullptr;
	freopen_s(&file, "CONOUT$", "w", stdout);
	freopen_s(&file, "CONOUT$", "w", stderr);
	freopen_s(&file, "CONIN$", "r", stdin);

	std::cout << "Injected!\n";
	
	if (!Hook::Init()) {
		Hook::Shutdown();
		ShutdownThread(file, "Hook failed to init");
	}

	while (!GetAsyncKeyState(VK_F1))
	{
		Sleep(25);
	}

	ShutdownThread(file);
	system("pause");
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved) {
	if (dwReason == DLL_PROCESS_ATTACH) {
		DllHandle = hModule;
		DisableThreadLibraryCalls(hModule);
		CloseHandle(CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, NULL, 0, NULL));
	}

	return TRUE;
}
