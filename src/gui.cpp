#include <Windows.h>
#include <iostream>

#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_win32.h"

#include "gui.h"

bool GUI::Init(HWND wnd_handle) {

    if (GUI::initialization) 
        return false;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui_ImplWin32_Init(wnd_handle);
    ImGui_ImplOpenGL3_Init();
    
    GUI::initialization = true;

    return true;
}

void GUI::Draw() {
    if (!GUI::draw)
        return;

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("OpenGL-Hook");
    
    ImGui::Text("Hello, World!");
    ImGui::Button("Click me to get aimbot lol");

    ImGui::End();

    ImGui::EndFrame();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUI::Shutdown() {
    if (!GUI::initialization)
        return;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
    GUI::initialization = false;
}

bool GUI::GetInitializationState() {
    return GUI::initialization;
}

bool GUI::GetDrawState() {
    return GUI::draw;
}

void GUI::SetDrawState(bool new_value) {
    GUI::draw = new_value;
}

