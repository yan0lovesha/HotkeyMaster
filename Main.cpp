#include <format>
#include <windows.h>
#include <iostream>
#include <string>
#include <cstdlib>
#include <Lmcons.h>
#include "Operations.h"

#define HotkeyID_CenterPointer 1

std::wstring GetApplicationPath() {
	wchar_t buffer[MAX_PATH];
	GetModuleFileName(NULL, buffer, MAX_PATH);
	std::wstring path(buffer);
	return path;
}

void RunnCommandInProcess(std::wstring& cmd) {
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	//si.wShowWindow = SW_HIDE;

	if (CreateProcessW(NULL, &cmd[0], NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
}

void EndScheduledTask() {
	std::wstring command = L"schtasks /end /tn \"HotKeyMaster\"";
	RunnCommandInProcess(command);
}

void DeleteScheduledTask() {
	EndScheduledTask();
	std::wstring command = L"schtasks /delete /tn \"HotKeyMaster\" /f";
	RunnCommandInProcess(command);
}

void AddScheduledTask(std::wstring applicationPath) {
	DeleteScheduledTask();

	wchar_t username[UNLEN + 1];
	DWORD username_len = UNLEN + 1;
	GetUserNameW(username, &username_len);

	std::wstring command = std::format(L"schtasks /create /tn \"HotKeyMaster\" /tr \"{}\" /sc onlogon /ru \"{}\" /rl HIGHEST", applicationPath, username);
	RunnCommandInProcess(command);
}

void RunScheduledTask() {
	std::wstring command = L"schtasks /run /tn \"HotKeyMaster\"";
	RunnCommandInProcess(command);
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
		//AddApplicationToStartup(path);
		AddScheduledTask(path);
		MessageBox(NULL, L"Added to startup.", L"Info", MB_OK | MB_ICONINFORMATION);
		RunScheduledTask();
		return 0;
	}

	if (lstrcmpiW(lpCmdLine, L"removeStartup") == 0) {
		DeleteScheduledTask();
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