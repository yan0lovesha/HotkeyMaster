#include <windows.h>
#include <iostream>
#include "Operations.h"

#define HotkeyID_CenterPointer 1

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	// Register the hotkey
	if (!RegisterHotKey(NULL, HotkeyID_CenterPointer, MOD_ALT | MOD_CONTROL, 'Y'))
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