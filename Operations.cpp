#pragma once

#include <windows.h>

void MovePointerToCenterOfActiveWindow()
{
    // Get the active window
    HWND activeWindow = GetForegroundWindow();
    if (activeWindow)
    {
        // Get the window dimensions
        RECT windowRect;
        GetWindowRect(activeWindow, &windowRect);
        int width = windowRect.right - windowRect.left;
        int height = windowRect.bottom - windowRect.top;

        // Calculate the center point
        int centerX = windowRect.left + width / 2;
        int centerY = windowRect.top + height / 2;

        // Move the mouse to the center of the active window
        SetCursorPos(centerX, centerY);
    }
}