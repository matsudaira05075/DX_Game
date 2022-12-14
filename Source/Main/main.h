#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#define NOMINMAX
#include <iostream>
#include <windows.h>
#include <assert.h>
#include <string>
#include <chrono>

#pragma warning(push)
#pragma warning(disable:4005)

#include <d3d11.h>
#include <d3dx9.h>
#include <d3dx11.h>

#pragma warning(pop)

#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3dx11.lib")

// Imgui
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#define SCREEN_WIDTH	(1296)			// ウインドウの幅
#define SCREEN_HEIGHT	(729)			// ウインドウの高さ
#define WINDOW_CLASS_NAME "AppClass"

HWND GetWindow();
HINSTANCE GetHinstance();

void DebugOutputString(const char* str, ...);