#include <windows.h>
#include <iostream>
#include <string>
#include "Operations.h"

#define HotkeyID_CenterPointer 1

std::wstring GetApplicationPath() {
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring path(buffer);
	return path;
}

void AddApplicationToStartup(std::wstring applicationPath) {
	HKEY hkey = NULL;
	LONG createStatus = RegCreateKey(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", &hkey);
	LONG status = RegSetValueEx(hkey, L"HotKeyMaster", 0, REG_SZ, (BYTE*)applicationPath.c_str(), (DWORD)(applicationPath.size() + 1) * sizeof(wchar_t));
	if (createStatus != ERROR_SUCCESS || status != ERROR_SUCCESS) {
		MessageBox(NULL, L"Error: Could not add application to startup.", L"Error", MB_OK | MB_ICONERROR);
	}
	else {
		// MessageBox(NULL, L"Success: Application added to startup.", L"Succeed", MB_OK | MB_ICONINFORMATION);
	}
	RegCloseKey(hkey);
}

void RemoveApplicationFromStartup() {
	HKEY hkey = NULL;
	LONG openStatus = RegOpenKeyEx(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_WRITE, &hkey);
	LONG deleteStatus = RegDeleteValue(hkey, L"HotKeyMaster");
	if (openStatus != ERROR_SUCCESS || deleteStatus != ERROR_SUCCESS) {
		MessageBox(NULL, L"Error: Could not remove application from startup.", L"Error", MB_OK | MB_ICONERROR);
	}
	else {
		// MessageBox(NULL, L"Success: Application removed from startup.", L"Succeed", MB_OK | MB_ICONINFORMATION);
	}
	RegCloseKey(hkey);
}

int wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nShowCmd)
{
	if (lstrcmpiW(lpCmdLine, L"addStartUp") == 0) {
		std::wstring path = GetApplicationPath();
		AddApplicationToStartup(path);
		MessageBox(NULL, L"Added to startup.", L"Info", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	if (lstrcmpiW(lpCmdLine, L"removeStartup") == 0) {
		RemoveApplicationFromStartup();
		MessageBox(NULL, L"Removed from startup.", L"Info", MB_OK | MB_ICONINFORMATION);
		return 0;
	}

	// Register the hotkey
	if (!RegisterHotKey(NULL, HotkeyID_CenterPointer, MOD_ALT | MOD_CONTROL | MOD_SHIFT, VK_F13))
	{
		MessageBox(NULL, L"Failed to register hotkey", L"Error", MB_OK | MB_ICONERROR);
		return -1;
	}

	// Message loop to wait for hotkey press
	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			if (msg.wParam == HotkeyID_CenterPointer) {
				MovePointerToCenterOfActiveWindow();
			}

		}
	}

	return 0;
}