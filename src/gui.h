#ifndef GUI_H_
#define GUI_H_

#include <Windows.h>
#include <iostream>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_win32.h"

namespace GUI {
	inline bool initialization;
	inline bool draw = true;

	bool Init(HWND wnd_handle);
	void Draw();
	void Shutdown();
	bool GetInitializationState();
	bool GetDrawState();
	void SetDrawState(bool new_value);
}

#endif