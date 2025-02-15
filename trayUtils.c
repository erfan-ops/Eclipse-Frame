#include "trayUtils.h"
#include "resource.h"


// Function to add the tray icon
void AddTrayIcon(const HWND* hwnd, const HICON* hIcon, const wchar_t* tooltip) {
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = *hwnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = *hIcon;
    wcsncpy_s(nid.szTip, _countof(nid.szTip) - 1, tooltip, _TRUNCATE);
    Shell_NotifyIconW(NIM_ADD, &nid);
}

// Function to remove the tray icon
static void RemoveTrayIcon(const HWND* hwnd) {
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = *hwnd;
    nid.uID = TRAY_ICON_ID;
    Shell_NotifyIconW(NIM_DELETE, &nid);
}

// Function to handle quit logic
void OnQuit(HWND* hwnd, bool* running) {
    RemoveTrayIcon(hwnd);
    *running = false;
}
