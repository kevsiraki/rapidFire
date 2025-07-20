#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <windows.h>
#include <thread>
#include <string>

using namespace std;

// Global variables
HWND hKeyDropdown, hDelayInput, hStartButton, hStopButton, hStatusLabel, hDescriptionLabel;
int key = 1;
int delay = 100;
bool running = false;
thread rapidFireThread;

// Function declarations
void CreateMainWindow(HINSTANCE hInstance);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void StartRapidFire(HWND hwnd);
void StopRapidFire();
void SimulateRapidFire();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    CreateMainWindow(hInstance);

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void CreateMainWindow(HINSTANCE hInstance)
{
    const wchar_t CLASS_NAME[] = L"RapidFireWindowClass";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = CreateSolidBrush(RGB(45, 45, 48)); // Dark background

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Rapid Fire",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 350, 400, // Larger window size
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return;
    }

    // Create UI controls
    CreateWindow(L"STATIC", L"Select Mouse Button:", WS_VISIBLE | WS_CHILD, 10, 10, 320, 20, hwnd, NULL, NULL, NULL);

    hKeyDropdown = CreateWindow(L"COMBOBOX", L"", CBS_DROPDOWNLIST | WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 30, 320, 150, hwnd, NULL, NULL, NULL);
    SendMessage(hKeyDropdown, CB_ADDSTRING, 0, (LPARAM)L"Middle Mouse Button (MMB)");
    SendMessage(hKeyDropdown, CB_ADDSTRING, 0, (LPARAM)L"Mouse Button 4 (M4)");
    SendMessage(hKeyDropdown, CB_ADDSTRING, 0, (LPARAM)L"Mouse Button 5 (M5)");
    SendMessage(hKeyDropdown, CB_SETCURSEL, 0, 0);

    CreateWindow(L"STATIC", L"Delay time (in MS):", WS_VISIBLE | WS_CHILD, 10, 60, 320, 20, hwnd, NULL, NULL, NULL);
    hDelayInput = CreateWindow(L"EDIT", L"10", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 80, 320, 20, hwnd, NULL, NULL, NULL);

    hStartButton = CreateWindow(L"BUTTON", L"Start", WS_VISIBLE | WS_CHILD, 10, 110, 150, 30, hwnd, (HMENU)1, NULL, NULL);
    hStopButton = CreateWindow(L"BUTTON", L"Stop", WS_VISIBLE | WS_CHILD | WS_DISABLED, 180, 110, 150, 30, hwnd, (HMENU)2, NULL, NULL);

    hStatusLabel = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD, 10, 150, 320, 30, hwnd, NULL, NULL, NULL);
    hDescriptionLabel = CreateWindow(L"STATIC", L"This application allows you to simulate rapid mouse clicks with a selected delay time. Choose the mouse button to simulate and set the delay time (in milliseconds). Click 'Start' to begin and 'Stop' to end the simulation.", WS_VISIBLE | WS_CHILD, 10, 190, 320, 100, hwnd, NULL, NULL, NULL);

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case 1: // Start button
            StartRapidFire(hwnd);
            break;
        case 2: // Stop button
            StopRapidFire();
            break;
        }
        break;

    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetTextColor(hdcStatic, RGB(255, 255, 255)); // White text
        SetBkColor(hdcStatic, RGB(45, 45, 48)); // Match background
        return (LRESULT)GetStockObject(NULL_BRUSH);
    }

    case WM_DESTROY:
        StopRapidFire();
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

void StartRapidFire(HWND hwnd)
{
    if (!running)
    {
        int index = SendMessage(hKeyDropdown, CB_GETCURSEL, 0, 0);
        switch (index)
        {
        case 0:
            key = 1; // Mouse Button 1 (MMB)
            break;
        case 1:
            key = 2; // Mouse Button 2 (M4)
            break;
        case 2:
            key = 3; // Mouse Button 3 (M5)
            break;
        }

        wchar_t delayText[10];
        GetWindowText(hDelayInput, delayText, 10);
        delay = _wtoi(delayText);

        running = true;
        rapidFireThread = thread(SimulateRapidFire);

        // Update UI
        EnableWindow(hStartButton, FALSE);
        EnableWindow(hStopButton, TRUE);
        SetWindowText(hStatusLabel, L"Rapid Fire running...");
    }
}

void StopRapidFire()
{
    if (running)
    {
        running = false;
        if (rapidFireThread.joinable())
        {
            rapidFireThread.join();
        }

        // Update UI
        EnableWindow(hStartButton, TRUE);
        EnableWindow(hStopButton, FALSE);
        SetWindowText(hStatusLabel, L"Rapid Fire stopped.");
    }
}

void SimulateRapidFire()
{
    while (running)
    {
        switch (key)
        {
        case 1: // MMB
            if (GetAsyncKeyState(VK_MBUTTON) & 0x8000)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(delay);
            }
            break;
        case 2: // M4
            if (GetAsyncKeyState(VK_XBUTTON1) & 0x8000)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(delay);
            }
            break;
        case 3: // M5
            if (GetAsyncKeyState(VK_XBUTTON2) & 0x8000)
            {
                mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
                mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
                Sleep(delay);
            }
            break;
        }

        Sleep(1); // Adjusted sleep time
    }
}
