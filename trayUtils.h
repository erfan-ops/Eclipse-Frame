#pragma once

#include <iostream>
#include <Windows.h>

#define WM_TRAYICON (WM_USER + 20)
#define TRAY_ICON_ID 1

// Load the icon from resources
extern "C" HICON LoadIconFromResource();

// Function to add the tray icon
void AddTrayIcon(HWND hwnd, HICON hIcon, const std::wstring& tooltip);

// Function to remove the tray icon
void RemoveTrayIcon(HWND hwnd);

// Function to handle quit logic
void OnQuit(HWND& hwnd, bool& running);
