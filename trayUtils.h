#pragma once

#include <stdbool.h>
#include <Windows.h>

#define WM_TRAYICON (WM_USER + 20)
#define TRAY_ICON_ID 1;

#ifdef __cplusplus
extern "C" {
#endif

// Load the icon from resources
extern HICON LoadIconFromResource();

// Function to add the tray icon
void AddTrayIcon(const HWND* hwnd, const HICON* hIcon, const wchar_t* tooltip);

// Function to handle quit logic
void OnQuit(HWND* hwnd, bool* running);

#ifdef __cplusplus
}
#endif