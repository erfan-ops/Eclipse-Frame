#include "trayUtils.h"
#include "resource.h"

#define WM_TRAYICON (WM_USER + 20)
#define TRAY_ICON_ID 1


// Function to add the tray icon
void AddTrayIcon(HWND hwnd, HICON hIcon, const std::wstring& tooltip) {
    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = hIcon;
    wcsncpy_s(nid.szTip, tooltip.c_str(), tooltip.size());
    Shell_NotifyIconW(NIM_ADD, &nid);
}

// Function to remove the tray icon
void RemoveTrayIcon(HWND hwnd) {
    NOTIFYICONDATA nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = TRAY_ICON_ID;
    Shell_NotifyIconW(NIM_DELETE, &nid);
}

// Function to handle quit logic
void OnQuit(HWND& hwnd, bool& running) {
    RemoveTrayIcon(hwnd);
    running = false;
}
