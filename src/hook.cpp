#include <Windows.h>
#include <iostream>
#include "MinHook.h"
#include "hook.h"
#include "../opengl/include/glad/glad.h"
#include "gui.h"

using wglSwapBuffers_t = BOOL(__stdcall*)(HDC hDc);
wglSwapBuffers_t original_wglSwapBuffers;
static HWND wnd_handle;
static WNDPROC origin_wndproc;
LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

BOOL __stdcall wglSwapBuffers_Func(HDC hDc)
{
	static bool init;

	if (init == false) {
		GUI::Init(wnd_handle);
		init = true;
	}
	else {
		GUI::Draw();
	}

	return original_wglSwapBuffers(hDc);
}


bool Hook::Init() {

	wnd_handle = FindWindowA("LWJGL", nullptr);

	if (!wnd_handle)
	{
		wnd_handle = FindWindowA("GLFW30", nullptr);

		if (!wnd_handle)
		{
			std::cout << "Failed to get window handle\n";
			return false;
		}
	}

	origin_wndproc = (WNDPROC)SetWindowLongPtrW(wnd_handle, GWLP_WNDPROC, (LONG_PTR)WndProc);
	if (MH_Initialize() != MH_OK) {
		std::cout << "Failed to initialize minhook\n";
		return false;
	}
	LPVOID wglSwapBuffers_address = reinterpret_cast<LPVOID>(GetProcAddress(GetModuleHandleA("OPENGL32.dll"), "wglSwapBuffers"));
	MH_CreateHook(wglSwapBuffers_address, reinterpret_cast<LPVOID>(&wglSwapBuffers_Func), reinterpret_cast<LPVOID*>(&original_wglSwapBuffers));
	MH_EnableHook(MH_ALL_HOOKS);
	return true;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (GUI::GetInitializationState())
	{
		if (msg == WM_KEYDOWN && wParam == VK_F2)
			GUI::SetDrawState(!GUI::GetDrawState());

		if (GUI::GetDrawState() && ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
			return true;
	}

	return CallWindowProcA(origin_wndproc, hWnd, msg, wParam, lParam);
}

bool Hook::GetState() {
	return Hook::initialize;
}

void Hook::Shutdown() {
	SetWindowLongPtrW(wnd_handle, GWLP_WNDPROC, (LONG_PTR)origin_wndproc);
	MH_Uninitialize();
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
}



